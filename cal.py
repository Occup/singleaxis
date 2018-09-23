#!/usr/bin/env python

import numpy as np
import numpy.linalg as lg

J=13
D1=1.1
D2=0.3
w1=0.7
w2=4
e1=0.005
e2=0.005
Tp=0.14
Tf=0.07

M=np.matrix([[J,D1,D2],[D1,1,0],[D2,0,1]])
print(M)
D=np.matrix([[0,0,0],[0,2*e1*w1,0],[0,0,2*e2*w2]])
print(D)
K=np.matrix([[0,0,0],[0,w1**2,0],[0,0,w2**2]])
print(K)
R=np.matrix([[1],[0],[0]])
print(R)

A = np.vstack([np.hstack([np.zeros([3,3]),np.eye(3)]),np.hstack([-lg.inv(M)*K,-lg.inv(M)*D])])
B = np.vstack([np.zeros([3,1]),lg.inv(M)*R])

print(A)
print(B)