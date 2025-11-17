import requests
import cv2
import numpy as np
import mediapipe as mp
import time

ESP32_STREAM = "http://192.168.101.7/"
ESP32_ALERT  = "http://192.168.101.7:81/alert"

MIN_HEIGHT_PX   = 200     
ALERT_COOLDOWN  = 0.5 
last_alert_time = 0

mp_pose = mp.solutions.pose
mp_drawing = mp.solutions.drawing_utils
pose = mp_pose.Pose(
    static_image_mode=False,
    model_complexity=1,
    enable_segmentation=False,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)

stream = requests.get(ESP32_STREAM, stream=True)
bytes_data = b""

print("📷 Conectado ao ESP32-CAM...")

while True:
    for chunk in stream.iter_content(chunk_size=1024):
        bytes_data += chunk

        start = bytes_data.find(b'\xff\xd8')
        end   = bytes_data.find(b'\xff\xd9')

        if start != -1 and end != -1:
            jpg = bytes_data[start:end+2]
            bytes_data = bytes_data[end+2:]

            frame = cv2.imdecode(
                np.frombuffer(jpg, dtype=np.uint8),
                cv2.IMREAD_COLOR
            )

            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            result = pose.process(rgb)

            if result.pose_landmarks:
                mp_drawing.draw_landmarks(
                    frame,
                    result.pose_landmarks,
                    mp_pose.POSE_CONNECTIONS,
                    mp_drawing.DrawingSpec(color=(0,255,0), thickness=2, circle_radius=2),
                    mp_drawing.DrawingSpec(color=(0,0,255), thickness=2)
                )

                h, w, _ = frame.shape
                landmarks = result.pose_landmarks.landmark

                x_top = int(landmarks[0].x * w)
                y_top = int(landmarks[0].y * h)

                x_ankle = int(landmarks[28].x * w)
                y_ankle = int(landmarks[28].y * h)

                height_pixels = abs(y_ankle - y_top)

                cv2.putText(
                    frame,
                    f"Altura (px): {height_pixels}",
                    (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    (0,255,0),
                    2
                )

                if (
                    height_pixels < MIN_HEIGHT_PX and
                    time.time() - last_alert_time > ALERT_COOLDOWN
                ):
                    try:
                        requests.get(ESP32_ALERT, timeout=0.2)
                        last_alert_time = time.time()
                        print(f"🔔 ALERTA: pouca altura ({height_pixels} px)")
                    except requests.RequestException:
                        pass

            cv2.imshow("ESP32 + MediaPipe Pose", frame)

            if cv2.waitKey(1) == ord("q"):
                break

cv2.destroyAllWindows()