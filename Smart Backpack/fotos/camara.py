from PIL import Image
import requests
from io import BytesIO
import os, sys
import datetime

datetime_object = datetime.datetime.now()
print(datetime_object)
d1 = str(datetime_object)
output = d1.replace(":","")
output = output.replace(" ","_")
output = output[0:17]+".jpg"

#Cambiar la direccion IP segun su configuracion
url = "http://192.168.1.172/1024x768.jpg"

response = requests.get(url)
img = Image.open(BytesIO(response.content))

try:
    img.save(output)
except IOError:
    print("cannot convert", infile)

datetime_object = datetime.datetime.now()
print(datetime_object) 