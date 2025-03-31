import RPi.GPIO as GPIO
import time
import serial

# Pines del sensor ultrasónico
TRIG = 23
ECHO = 24

# Configuración de GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

# Configurar UART (ajustar si es necesario)
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.reset_output_buffer()

def medir_distancia():
    GPIO.output(TRIG, False)
    time.sleep(0.05)

    # Pulso de activación de 10us
    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    # Medir el tiempo de respuesta
    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()

    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distancia = pulse_duration * 17150  # en cm
    return round(distancia, 2)

try:
    while True:
        distancia = medir_distancia()
        print(f"Distancia: {distancia} cm")

        # Enviar como texto por UART (ej. "15\n")
        ser.write((str(int(distancia)) + "\n").encode('utf-8'))
        time.sleep(0.1)

except KeyboardInterrupt:
    GPIO.cleanup()
    ser.close()
