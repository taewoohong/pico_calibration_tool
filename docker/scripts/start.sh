#!/bin/bash

docker build -t test .

xhost +local:root

docker run -it --rm \
  --device=/dev/video0 \
  --device=/dev/bus/usb/ \
  -e DISPLAY=$DISPLAY \
  -e QT_X11_NO_MITSHM=1 \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  --privileged \
  --entrypoint /bin/bash test
