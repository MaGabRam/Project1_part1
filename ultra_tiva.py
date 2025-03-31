import serial

ser = serial.Serial('/dev/ttyACM0', 9600)
ser.reset_input_buffer()

while True:
    if ser.in_waiting:
        try:
            data = ser.readline().decode('utf-8').strip()
            distancia = float(data)
            print(f"Distancia recibida: {distancia} cm")
        except ValueError:
            print("Dato no válido:", data)
