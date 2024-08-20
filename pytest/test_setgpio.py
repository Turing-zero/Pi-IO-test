import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

GPIO.setup(25, GPIO.OUT)

# while True:
GPIO.output(25,GPIO.HIGH)
