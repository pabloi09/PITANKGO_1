#!/bin/bash
echo "Ejecutando camera script"
cd ~/mjpg/mjpg-streamer/
./mjpg_streamer -i "./input_uvc.so -d /dev/video0 -y -f 15" -o "./output_http.so -w ./www"