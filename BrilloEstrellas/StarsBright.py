#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug 22 10:55:51 2018

@author: david
"""
import numpy as np
#import matplotlib.pyplot as plt
#from astropy.convolution import Gaussian2DKernel,convolve
#import scipy.linalg as linalg
datosP=[10.55,76.2,88.83,130.23,379.21,742.25,3.78,258.56,8.32,4.51]
datosN=["canopus","capella","arcturus","vega","sirius","rigil","rigel","procycon","hadar","betelgeuse"]
datosM=[-0.72,-0.08,-0.04,-0.03,-1.46,-0.27,0.12,0.38,0.61,0.5]
datosDE=[0.56,0.46,0.44,0.36,1.58,1.4,0.34,1.26,0.5,0.8]
p=0.01
def dist(p):
    r=[]
    for elem in p:
        r.append(1000/elem)
    return r
def magnitud(d,m):
    r=[]
    for i in range(len(d)):
        r.append(m[i]-5*np.log10(d[i])+5)
    return r
def error(d,de):
    r=[]
    for i in range(len(d)):
        r.append((5/np.log(10))*(de[i]/d[i]))
    return r
def errorD(s,d):
    r=[]
    for i in range(len(d)):
        r.append((s[i]*1000)/(datosP[i]**2))
    return r
datosD=dist(datosP)
datosAM=magnitud(datosD,datosM)
datosED=errorD(datosDE,datosD)
datosE=error(datosD,datosED)

for i in range(len(datosD)):
    enun="Para "+datosN[i]+" la distancia es "+str(datosD[i])+"+-"+str(datosED[i])+" y la magnitud absoluta es "+str(datosAM[i])+"+-"+str((datosE[i]))
    print(enun)
tam=len(datosP)
print(np.sort(datosM))