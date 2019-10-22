#!/usr/bin/python -u
# coding=utf-8
# "DATASHEET": http://cl.ly/ekot
# Original (and heavily changed) code from: https://gist.github.com/kadamski/92653913a53baf9dd1a8
# By: Mauri Favaron

import os
import sys
import sqlite3


class database:
	
	def __init__(self, data_path):
		
		# Assume success (will falsify on failure)
		self.ret_code = 0
		self.msg      = ""
		
		# Try creating the DB (create it if not found)
		db_file = os.path.join(data_path, "pm_data.db")
		try:
			self.db = sqlite3.connect(db_file)
		except Exception as e:
			self.cr       = None
			self.ret_code = 1
			self.msg      = "Database %s not created" % db_file
			return
		self.cr = self.db.cursor()
		try:
			self.cr.execute("CREATE TABLE Dust (date text, pm_2_5 float, pm_10 float)")
		except Exception as e:
			self.ret_code = 2
			self.msg      = "Data table not created in database"
			return
		self.db.commit()
		

if __name__ == "__main__":
	
	# Test 1: Create database
	db = database("data")
	if db.ret_code != 0:
		print("Error: Database not created")
		sys.exit(1)
		
	print("Sun chì!")
