#!/bin/bash
#Script de inicio del streaming de video
echo "Ejecutando camera script"
#Nos situamos en el directorio del programa
cd ~/mjpg/mjpg-streamer/
#Iniciamos el streaming con los plugins de entrada y salida, eligiendo el dispositivo, el formato del video, los fps y la carpeta de recursos web
./mjpg_streamer -i "./input_uvc.so -d /dev/video0 -y -f 15" -o "./output_http.so -w ./www"