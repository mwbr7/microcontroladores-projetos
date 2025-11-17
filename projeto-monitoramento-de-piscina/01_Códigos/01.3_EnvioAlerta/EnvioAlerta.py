import requests
import cv2
import numpy as np
import time

ESP32_STREAM = "http://192.168.101.7/"
ESP32_ALERT = "http://192.168.101.7:81/alert"

face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
)

stream = requests.get(ESP32_STREAM, stream=True)
bytes_data = b""
last_alert_time = 0

for chunk in stream.iter_content(chunk_size=1024):
    bytes_data += chunk

    start = bytes_data.find(b'\xff\xd8')
    end   = bytes_data.find(b'\xff\xd9')

    if start != -1 and end != -1:
        jpg = bytes_data[start:end+2]
        bytes_data = bytes_data[end+2:]

        img = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.1, 5)

        if len(faces) > 0 and time.time() - last_alert_time > 0.5:
            try:
                r = requests.get(ESP32_ALERT, timeout=0.2)
                last_alert_time = time.time()
            except:
                pass  
        for (x, y, w, h) in faces:
            cv2.rectangle(img, (x, y), (x+w, y+h), (0,255,0), 2)

        cv2.imshow("ESP32 Stream", img)

        if cv2.waitKey(1) == ord('q'):
            break

cv2.destroyAllWindows()