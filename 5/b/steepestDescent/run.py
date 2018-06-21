#!/usr/bin/python3
import os

rel_path = '../../../color-instances/'
for filename in os.listdir(rel_path):
	name = filename.split('.')
	exe_str = './p1b.out ' + rel_path + filename + ' > output/' + name[0] + '.output' 
	print(exe_str)
	os.system(exe_str)
