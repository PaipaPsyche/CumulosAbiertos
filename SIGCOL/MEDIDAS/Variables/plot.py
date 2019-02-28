"""
Created on Wed Nov  7 13:06:08 2018

@author: david
"""
import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import PolynomialFeatures
import pandas as pd

#from astropy.convolution import Gaussian2DKernel,convolve
#import scipy.linalg as linalg
#from mpl_toolkits.mplot3d import Axes3D
DATA=pd.read_csv("SIGCOL_RUN.txt",delimiter=" ",header=None)

x=DATA[2].values
y=DATA[3].values
names=DATA[0].values


#plt.scatter(x,y)

ii=x>np.amin(x)


x=x[ii]
y=y[ii]



n_bins=15
tam_x=np.amax(x)-np.amin(x)
l_bin=tam_x/n_bins

n_x=np.zeros(n_bins)
n_y=np.zeros(n_bins)

for i in range(n_bins):
    n_x[i]=np.amin(x)+i*l_bin
    ii=np.logical_and(x>=n_x[i],x<n_x[i]+l_bin)
    n_y[i]=0
    if(len(y[ii])!=0):
        n_y[i]=np.mean(y[ii])


deg=5
poly = PolynomialFeatures(degree=deg)
X = poly.fit_transform(n_x.reshape(-1,1))
pred_x=poly.fit_transform(x.reshape(-1,1))
act_y=poly.fit_transform(y.reshape(-1,1))
Y=poly.fit_transform(n_y.reshape(-1,1))
clf = linear_model.LinearRegression()
clf.fit(X,Y)
pred_y=clf.predict(pred_x)

TH=2
rev=1

ii=TH*pred_y[:,1]<=act_y[:,1]
jj=np.logical_not(ii)

indexes=np.where(ii)[0]
#variabs=names[ii]


fileout=open("VAR_"+str(deg)+".txt","w")
for i in indexes:
    fileout.write(names[i]+" "+str(x[i])+" " +str(y[i]))
    fileout.write("\n")
fileout.close()


fileout=open("VAR_"+str(deg)+"_names.txt","w")
for i in indexes:
    fileout.write(names[i])
    fileout.write("\n")
fileout.close()



plt.figure(figsize=[10,10])
plt.title("Numero de candidatos = "+str(len(x[ii])))
plt.scatter(x[ii],y[ii],c="g",alpha=0.2)
print(len(x[ii]), " estrellas candidatas.")
plt.scatter(x[jj],y[jj],c="r",alpha=0.2)


t="Y = "
for i in range (len(clf.coef_[rev])):
    t=t+str(round(clf.coef_[rev][i]*10**1,2))+"x $10^{-10}$ "+"$X^{"+str(i+1)+"}$  +  "
t=t+str(round(clf.intercept_[rev]*10**1,2))+"x $10^{-10}$ "
plt.title(t,fontsize=int(65/len(clf.coef_[rev])))


plt.scatter(x,TH*pred_y[:,rev],s=2)
plt.savefig("FIT_"+str(deg)+"sigc.png")



#plt.scatter(x,y)
