BOARD?=esp8266:esp8266:nodemcuv2
PORT?=/dev/ttyACM0

all:
	arduino-cli compile -board $(BOARD)
flash:
	arduino-cli upload -board $(BOARD) -port $(PORT)
