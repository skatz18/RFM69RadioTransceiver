from RFM69 import Radio, FREQ_915MHZ
import datetime
import time
import sys
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
pin = 12
GPIO.setup(pin,GPIO.OUT)
node_id = 2
network_id = 100
recipient_id = 1

def led(state):
	if state == True:
		GPIO.output(pin,GPIO.HIGH)
	else:
		GPIO.output(pin,GPIO.LOW)
	return

with Radio(FREQ_915MHZ, node_id, network_id, isHighPower=True, promiscuousMode=True) as radio:
	print("Starting Loop...")

	rx_counter = 0
	tx_counter = 0

	while True:

		#Receive Packets
		if(radio.has_received_packet()):
			packet=radio.get_packets()
			if(len(packet[0].data)==5):
				print("Packet Received")
				print (packet[0].data)
				btnval = packet[0].data[0]
				xmostsig = packet[0].data[1]
				xleastsig = packet[0].data[2]
				ymostsig = packet[0].data[3]
				yleastsig = packet[0].data[4]
				xval = xmostsig << 8 | xleastsig
				yval = ymostsig << 8 | yleastsig
				print("btnval: ", btnval, "xval: ", xval, "yval: ", yval)
				if((xval == 510 or  xval ==511)and(yval==518 or yval==517)):
					print("staionary")
				if(xval > 511):
					print("moving right")
					if (xval > 750):
						print("fast")
				if(xval<510):
					print("moving left")
					if (xval<250):
						print("fast")
				if(yval >518):
					print("moving forward")
					if(yval > 750):
						print("fast")
				if(yval<517):
					print("moving backward")
					if(yval<250):
						print("fast")
			
				led(True)
				radio.send(recipient_id,"")

		else:
			time.sleep(.5)
			print("Listening")
			led(False)


