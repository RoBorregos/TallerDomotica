import requests
import serial

puerto = 'COM5'
arduino = serial.Serial(puerto,9600)
print 'Conectado a ', puerto
print 'Esperando instruccion...'
while 1:
	if arduino.in_waiting() > 0:
		valor = arduino.read()
		if valor == 1:
			r = requests.post('https://maker.ifttt.com/trigger/in_jubileo/with/key/g8S-6rJSiz6aivZAaoZwtbdNPToi7z6Ktatcv57e9G1')
			arduino.write(2)
		else:
			print 'Comando no reconocido'