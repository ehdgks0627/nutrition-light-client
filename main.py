import RPi.GPIO as gpio

gpio.setmode(gpio.BCM)

#init pins
gpio.setup(27, gipo.OUT)
gpio.setup(28, gipo.IN)

try:
    pass
except KeyboardInterrupt:
    gpio.cleanup()
