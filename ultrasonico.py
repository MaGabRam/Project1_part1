import RPi.GPIO as GPIO
import time

# Pines del sensor ultrasónico
TRIG = 23
ECHO = 24

# Configuración de GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)
# Asegura que el TRIG esté en bajo
GPIO.output(TRIG, GPIO.LOW)
time.sleep(0.05)
def medir_distancia():
    # Pulso de 10us para activar el sensor
    GPIO.output(TRIG, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG, GPIO.LOW)

    # Espera a que ECHO suba
    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()

    # Espera a que ECHO baje
    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    # Calcular duración y distancia
    pulse_duration = pulse_end - pulse_start
    distancia = pulse_duration * 17150
    distancia = round(distancia, 2)

    return distancia

try:
    while True:
        d = medir_distancia()
        print(f"Distancia: {d} cm")
        time.sleep(0.5)

except KeyboardInterrupt:
    GPIO.cleanup()
