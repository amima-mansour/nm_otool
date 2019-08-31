#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
import signal
import subprocess
import argparse
import textwrap


# absolute path to your nm_otool project
PROJECT_PATH = '/Users/dogokar/nm_otool/'

VALGRPATH = '/Users/dogokar/.brew/bin/valgrind'
NM_PATH = os.path.join(PROJECT_PATH, 'ft_nm')
FINALLINE = os.path.join(VALGRPATH, NM_PATH)
OTOOL_PATH = os.path.join(PROJECT_PATH, 'ft_otool')

def create_seg(name_file):
  fichier = open("data_error.txt", "a")
  fichier.write(name_file + '\n')
  fichier.close()


def execute(cmd):
    print(cmd)
    proc = subprocess.Popen(cmd, shell=True, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = proc.communicate()
    output = output.decode('unicode-escape').rstrip()
    error = error.decode('unicode-escape').rstrip()
    rc = proc.returncode
    return (output, rc, error)
    
def check_leaks(argv):
    for arg in sys.argv:
       out, rc, error = execute([VALGRPATH + ' ' + OTOOL_PATH + ' ' + arg])
       if error.find("definitely lost: 0 bytes in 0 blocks") == -1:
            create_seg(arg)
            print(error)

def tests_main():
    #out, rc, error = execute([VALGRPATH + ' ./ft_nm'])
    check_leaks(sys.argv)
    #print(out)
    #print(rc)
    #print(error)
    



if __name__ == '__main__':
    
    sys.exit(tests_main())
