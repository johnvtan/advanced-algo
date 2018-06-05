#!/usr/bin/python3
import os

# writes text to file
def write_file(filename, text):
    with open(filename, 'w') as f:
        for line in text:
            f.write(line + '\n')

if __name__ == '__main__':
    rel_path = '../../color-ampl/'
    problem = 'p3b'

    # text to generate new run file for each knapsack instance
    run_file_text = ['option solver cplex;', 
                 'option cplex_options \'time=600\';', 
                 'model color.mod;',
                 '',
                 'solve;',
                 'display colors;',
                 'display objective;']

    run_file_name = 'knapsack.run'

    os.system('mkdir -p output')
    os.system('rm output/*')    # get rid of existing output from previous runs
    
    for filename in os.listdir(rel_path):
        name = filename.split('.')
        output_name = 'output/' + name[0] + '.out'
        run_file_text[3] = 'data ' + rel_path + filename
        write_file(run_file_name, run_file_text)
        exe_str = 'ampl ' + run_file_name + ' > ' + output_name
        print(exe_str)
        os.system(exe_str)
