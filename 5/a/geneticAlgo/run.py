#!/usr/bin/python3
import os

rel_path = '../../../knapsack-instances/'
problem = 'p1a'

os.system('mkdir -p output')
os.system('rm output/*')    # get rid of existing output from previous runs
for filename in os.listdir(rel_path):
    name = filename.split('.')
    exe_str = './' + problem + '.out ' + rel_path + filename + ' > output/' + name[0] + '.output' 
    print(exe_str)
    os.system(exe_str)