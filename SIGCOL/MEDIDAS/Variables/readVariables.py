"""
Created on Tue Nov 20 14:48:04 2018

@author: david
"""
import numpy as np
import sys
names=sys.argv[1]
TH=float(sys.argv[2])
c=0
ctot=0
#    #filename="FN.1043.WFI.dat"
#NAMES=np.genfromtxt(names)
with open(names) as f:
    NAMES = f.readlines()
NAMES = [x.strip() for x in NAMES] 


def reading(FN):
    LINES=[]
    ANS=[]
    with open(FN) as f:
        LINES = f.readlines()
    for line in LINES:
        l=line.split()
        
        l=l[1]
        l=float(l)
        ANS.append(l)

    return np.amax(ANS)

    #  

#
#
for filename in NAMES:
    ctot=ctot+1
    ANS=reading(filename)
    if(ANS>=TH):
        print("Nueva Candidata : ",filename,"  P = ",ANS)
        c=c+1
        FILE=open("SUSPVAR.txt","a+")
        FILE.write(filename[3:]+" "+str(ANS)+"\n")
        FILE.close()
        FILE=open("SUSPVARNAMES.txt","a+")
        FILE.write(filename[3:]+"\n")
        FILE.close()


print("Se encontraron "+str(c)+" estrellas candidatas a estrellas variables.")
print("Corresponden al "+str(round(c*100/ctot,2))+"% de los casos estudiados.")
#        
#        
#

#    
