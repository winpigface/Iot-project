#! usr/bin/sh

arduino-cli compile -b esp8266:esp8266:nodemcuv2
arduino-cli upload -b esp8266:esp8266:nodemcuv2 -p /dev/ttyACM0
