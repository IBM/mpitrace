#!/usr/bin/env python3
import sys
import subprocess
import io

if len(sys.argv) != 2:
    sys.exit("syntax : translate.py mpi_profile.jobid.rank")

file = open(sys.argv[1])
line = file.readline()
while line:
    line = line.rstrip()
    print(line)
    val = line.find('  filename = ')
    if val == 0:
        exe_or_lib = line[13:]
        lib = exe_or_lib.find('.so')
        next = file.readline()
        next = next.rstrip()
        print(next)
        if lib > 0:
            indx = next.find('offset  =')
            arg  = next[indx+10:]
        else:
            indx = next.find('address =')
            arg  = next[indx+10:indx+28]
        result = subprocess.run(['addr2line', '-f', '-C', '-e', exe_or_lib, arg], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output = result.stdout.decode('utf-8')
        buffer = io.StringIO(output)
        myline = '    function = ' + buffer.readline().strip()
        print(myline)
        myline = '    source = ' + buffer.readline().strip()
        print(myline)
    line = file.readline()
file.close
