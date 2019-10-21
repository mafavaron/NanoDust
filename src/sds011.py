#!/usr/bin/python
# coding=utf-8

import serial, struct, sys, time

class SDS011:

	# Class attributes
	CMD_MODE           = 2
	CMD_QUERY_DATA     = 4
	CMD_DEVICE_ID      = 5
	CMD_SLEEP          = 6
	CMD_FIRMWARE       = 7
	CMD_WORKING_PERIOD = 8
	MODE_ACTIVE        = 0
	MODE_QUERY         = 1
	PERIOD_CONTINUOUS  = 0

	def __init__(self):
		
		# Configre and initialize serial port
		self.ser = serial.Serial()
		self.ser.port = "/dev/ttyUSB0"
		self.ser.baudrate = 9600
		self.ser.open()
		self.ser.flushInput()
		
		# Initialize internal self.data
		self.byte = 0
		self.data = ""

	def dump(self, d, prefix=''):
		print(prefix + ' '.join(x.encode('hex') for x in d))

	def construct_command(self, cmd, data=[]):
		assert len(data) <= 12
		data += [0,]*(12-len(data))
		checksum = (sum(data)+cmd-2)%256
		ret = "\xaa\xb4" + chr(cmd)
		ret += ''.join(chr(x) for x in data)
		ret += "\xff\xff" + chr(checksum) + "\xab"

		return ret

	def process_data(self, d):
		r = struct.unpack('<HHxxBB', d[2:])
		pm25 = r[0]/10.0
		pm10 = r[1]/10.0
		checksum = sum(ord(v) for v in d[2:8])%256
		return [pm25, pm10]
		#print("PM 2.5: {} μg/m^3  PM 10: {} μg/m^3 CRC={}".format(pm25, pm10, "OK" if (checksum==r[2] and r[3]==0xab) else "NOK"))

	def process_version(self, d):
		r = struct.unpack('<BBBHBB', d[3:])
		checksum = sum(ord(v) for v in d[2:8])%256
		print("Y: {}, M: {}, D: {}, ID: {}, CRC={}".format(r[0], r[1], r[2], hex(r[3]), "OK" if (checksum==r[4] and r[5]==0xab) else "NOK"))

	def read_response(self):
		byte = 0
		while byte != "\xaa":
			byte = self.ser.read(size=1)

		d = self.ser.read(size=9)

		return byte + d

	def cmd_set_mode(self, mode=self.MODE_QUERY):
		self.ser.write(construct_command(self.CMD_MODE, [0x1, mode]))
		read_response()

	def cmd_query_data(self):
		self.ser.write(construct_command(self.CMD_QUERY_DATA))
		d = read_response()
		values = []
		if d[1] == "\xc0":
			values = process_data(d)
		return values

	def cmd_set_sleep(self, sleep):
		mode = 0 if sleep else 1
		self.ser.write(self.construct_command(self.CMD_SLEEP, [0x1, mode]))
		self.read_response()

	def cmd_set_working_period(self, period):
		self.ser.write(self.construct_command(self.CMD_WORKING_PERIOD, [0x1, period]))
		self.read_response()

	def cmd_firmware_ver(self):
		self.ser.write(self.construct_command(self.CMD_FIRMWARE))
		d = self.read_response()
		self.process_version(d)

	def cmd_set_id(self, id):
		id_h = (id>>8) % 256
		id_l = id % 256
		self.ser.write(self.construct_command(self.CMD_DEVICE_ID, [0]*10+[id_l, id_h]))
		self.read_response()

