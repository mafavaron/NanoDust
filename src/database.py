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
			self.ret_code = 0
			self.msg      = "Data table not created in database"
			return
		self.db.commit()
		
	
	def log_data(self, pm_2_5, pm_10):
		
		# Attempt writing and committing data (inefficient maybe, yet safe, and not too cumbersome with
		# the lightweight 'sqlite3')
		try:
			self.cr.execute("INSERT INTO Dust (date, pm_2_5, pm_10) VALUES (?, ?, ?)")
			self.db.commit()
		except Exception as e:
			self.ret_code = 3
			self.msg      = "Data value not written to table"
			return False
		return True
		

if __name__ == "__main__":
	
	# Test 1: Create database
	db = database("data")
	if db.ret_code != 0:
		print("Error: Database not created - " + db.msg)
		sys.exit(1)
	for i in range(10):
		fi = float(i)
		success = db.log_data(fi, -fi)
		if not success:
			print("Write failed on test 1 - " + db.msg)
			sys.exit(2)
		
	print("Sun chì!")
