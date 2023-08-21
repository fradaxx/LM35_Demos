# -*- coding: utf-8 -*-
"""
Created on Fri Dec 16 15:56:02 2022

@author: ProfFrancescoAdamo
"""

import serial
import matplotlib.pyplot as plt
from math import nan
import signal
import time
import readchar
import numpy as np


def handler(signum, frame):
    msg = "Ctrl-c was pressed. Do you really want to exit? y/n "
    print(msg, end="", flush=True)
    res = readchar.readchar()
    if res == 'y':
        print("")
        ser.close()
        exit(1)
    else:
        print("", end="\r", flush=True)
        print(" " * len(msg), end="", flush=True) # clear the printed line
        print("    ", end="\r", flush=True)
        

plt.ion()
fig=plt.figure()



serialPort = "COM6"
baudRate = 115200
historyLength = 100

i = historyLength - 1
x = np.linspace(0, historyLength - 1, historyLength)
x = x.tolist();
y = [nan] * historyLength
plt.scatter(x, y)
plt.ylim([10, 50])
plt.autoscale(False, 'both')
   
print("Opening Serial Port")
try:
    ser = serial.Serial(serialPort, baudRate)
    print("Serial Port is open")

except serial.SerialException:
    serial.Serial(serialPort, baudRate).close()
    ser = serial.Serial(serialPort, baudRate)
    
signal.signal(signal.SIGINT, handler)

while True:
    data = ser.readline()
    print(data)
    try:
        m = float(data.decode())
    except ValueError:
        m = nan
        
    #print(m)
    if (len(x) == historyLength):
        x.pop(0)
        y.pop(0)
        
    x.append(i)
    y.append(m)
    
    plt.scatter(x, y)
    i += 1
    plt.show()
    plt.pause(0.0001)  # Note this correction