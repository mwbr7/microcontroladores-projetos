import requests
import cv2
import numpy as np

ESP32_URL = "http://192.168.101.6/" # Substituir pelo IP.

face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

stream = requests.get(ESP32_URL, stream=True)
bytes_data = b""

for chunk in stream.iter_content(chunk_size=1024):
    bytes_data += chunk
    
    a = bytes_data.find(b'\xff\xd8')
    b = bytes_data.find(b'\xff\xd9')
    
    if a != -1 and b != -1:
        jpg = bytes_data[a:b+2]
        bytes_data = bytes_data[b+2:]
        
        img = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
        
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        
        for (x, y, w, h) in faces:
            cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)  # cor do retângulo: azul
        
        cv2.imshow("Detecção de Rostos", img)
        
        cv2.imwrite("foto_com_rostos.jpg", img)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):  # 'q' para sair
            break

cv2.destroyAllWindows()