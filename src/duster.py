#!/usr/bin/python -u
# coding=utf-8
# "DATASHEET": http://cl.ly/ekot
# Original (and heavily changed) code from: https://gist.github.com/kadamski/92653913a53baf9dd1a8
# By: Mauri Favaron
from __future__ import print_function
import sys, time
from sds011 import *

DATA_FILE = "../data/test.csv"


if __name__ == "__main__":
	
	# Get input parameters
	if len(sys.argv) != 3:
		print("duster.py - Procedure for collecting PM-2.5 and PM-10 from the low-cost SDS001 dust sensor")
		print()
		print("Usage:")
		print()
		print("  sudo ./duster.py <num_hours> <out_path>")
		print()
		print("Copyright 2019 by Servizi Territorio srl")
		print("                  All rights reserved")
		print()
		print("Written by: Mauri Favaron")
		print()
		sys.exit(1)
	num_hours = int(os.argv[1])
	out_path  = os.argv[2]
	
	# Check serial port is connected (it *must* be /dev/ttyUSB0)
	if not os.isdir("/dev/ttyUSB0"):
		print("duster.py:: error: Serial port '/dev/ttyUSB0' not found; check connections")
		sys.exit(2)
	
	# SDS011 activation sequence
	cmd_set_sleep(0)
	time.sleep(10)	# Allow measurements to settle
	cmd_firmware_ver()
	cmd_set_working_period(PERIOD_CONTINUOUS)
	cmd_set_mode(MODE_QUERY)
    
	# Get some samples
	f = open(DATA_FILE, "w")
	f.write("Time.Stamp, PM_2.5, PM_10\n")
	iNumIter = 250
	cmd_set_sleep(0)
	for iIter in range(iNumIter):
		
		# Get value
		values = cmd_query_data();
		if values is not None and len(values) == 2:
			print("PM2.5: ", values[0], ", PM10: ", values[1])
			time.sleep(2)

		# Store value
		f.write("%s, %f, %f\n" % (time.strftime("%Y-%m-%d %H:%M:%S"), values[0], values[1]))
		print(iIter)

	# Save file and leave
	f.close()
	cmd_set_sleep(1)

