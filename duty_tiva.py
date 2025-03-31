import RPi.GPIO as GPIO
import time
import serial
GPIO.setwarnings(False)
archivo = "duty.txt"
prev_duty=0
aux=0
def read_interval():
    with open(archivo, "r") as file:
        line = file.readline().strip()
        if line == '':
            print("⚠️ Archivo vacío, usando valor por defecto.")
            return 50
        try:
            interval = float(line)
            return max(0, interval)
        except ValueError:
            print(f"⚠️ Valor inválido: '{line}', usando 50.")
            return 50
#UART
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.reset_input_buffer()
try:
    while True:
        duty = read_interval()

        # Enviar solo si cambió el valor
        if (duty != prev_duty) and aux==0:
            print(f"Enviando duty: {duty}")
            ser.write((str(int(duty)) + "\n").encode('utf-8'))
            prev_duty = duty
            aux=1
        elif aux==1:
            ser.write((str(int(prev_duty)) + "\n").encode('utf-8'))
            aux=0


        # Leer respuesta opcional de la Tiva
        if ser.in_waiting:
            try:
                data = ser.readline().decode('utf-8').strip()
                print("Respuesta de Tiva:", data)
            except UnicodeDecodeError:
                print("⚠️ Datos UART corruptos")
        
        time.sleep(0.1)
except KeyboardInterrupt:
    print("\nSaliendo del programa.")
    ser.close()