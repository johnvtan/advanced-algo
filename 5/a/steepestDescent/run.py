#!/usr/bin/python3
import os

rel_path = '../../../knapsack-instances/'
for filename in os.listdir(rel_path):
	name = filename.split('.')
	exe_str = './p1a.out ' + rel_path + filename + ' > output/' + name[0] + '.output' 
	print(exe_str)
	os.system(exe_str)
