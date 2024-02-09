#Created by Trotiflex https://github.com/Trotiflex

import math

L1 = 5 #coxa
L2 = 6 #femur
L3 = 13.5 #tibia

x = 13 #Extension de la patte
z = 8 #Hauteur

def Racine(a, b):
    return math.sqrt((a*a) + (b*b))

def AlKashi(a, b, c):
    return math.acos((a*a +b*b - c*c) / (2*a*b))

def RadToDegrees(a):
    return a*(180/math.pi)

Hypotenuse = Racine(x, z)
print("Hypotenuse:",Hypotenuse)

A1 = AlKashi(z, Hypotenuse, x)
A2 = AlKashi(Hypotenuse, L2, L3)
A3 = AlKashi(L2, L3, Hypotenuse)

AngleFemur = 180 - (RadToDegrees(A1) + RadToDegrees(A2))

print("L'angle du femur est:", AngleFemur)