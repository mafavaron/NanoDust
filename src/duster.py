#!/usr/bin/python -u
# coding=utf-8
# "DATASHEET": http://cl.ly/ekot
# Original (and heavily changed) code from: https://gist.github.com/kadamski/92653913a53baf9dd1a8
# By: Mauri Favaron
from __future__ import print_function
import os, sys, time
import sds011


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
	num_hours = int(sys.argv[1])
	out_path  = sys.argv[2]
	
	# Check serial port is connected (it *must* be /dev/ttyUSB0)
	if not os.path.exists("/dev/ttyUSB0"):
		print("duster.py:: error: Serial port '/dev/ttyUSB0' not found; check connections")
		sys.exit(2)
		
	# Connect sensor
	sds = sds011.SDS011()
	
	# SDS011 activation sequence
	sds.cmd_set_sleep(0)
	time.sleep(10)	# Allow measurements to settle
	sds.cmd_firmware_ver()
	sds.cmd_set_working_period(sds.PERIOD_CONTINUOUS)
	sds.cmd_set_mode(sds.MODE_QUERY)
	
	# Build initial file name
	out_file = os.path.join(out_path, "Dust_%s.csv" % time.strftime("%Y%m%d.%H"))
	old_file = out_file
    
	# Get the samples desired
	f = open(out_file, "w")
	f.write("Time.Stamp, PM_2.5, PM_10\n")
	sds.cmd_set_sleep(0)
	while True:
		
		try:
			
			# Change file name, if needed
			out_file = os.path.join(out_path, "Dust_%s.csv" % time.strftime("%Y%m%d.%H"))
			if out_file != old_file:
				f.close()
				f = open(out_file, "w")
				f.write("Time.Stamp, PM_2.5, PM_10\n")
			
			# Get value
			values = sds.cmd_query_data();
			if values is not None and len(values) == 2:
				print(out_file, "  PM2.5: ", values[0], ", PM10: ", values[1])
				time.sleep(2)

			# Store value
			f.write("%s, %6.2f, %6.2f\n" % (time.strftime("%Y-%m-%d %H:%M:%S"), values[0], values[1]))
			f.flush()
			
		except e as KeyboardInterrupt:
			
			break

	# Save file and leave
	f.close()
	sds.cmd_set_sleep(1)

