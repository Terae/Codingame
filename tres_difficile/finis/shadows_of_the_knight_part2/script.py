import sys
import math

W,H = [int(v) for v in input().split()]
N = int(input())
X0,Y0 = [int(v) for v in input().split()]

X = X0
Y = Y0

TX = range(W)
TY = range(H)

def set_warmer(X0,X,Y0,Y):
    global TX,TY
    if X != X0:
        TX = [v for v in TX if abs(v-X)<abs(v-X0)]
    else:
        TY = [v for v in TY if abs(v-Y)<abs(v-Y0)]

def get_next(v, tab, vmax):
    res = tab[0]+tab[-1]-v

 # On fait en sorte que Batman ne fasse pas de surplace
    if v == res:
        res += 1
    res = min(max(res,0),vmax)

   # Si on est au bord du bâtiment : on récupère la moitié de la distance
    if v==0 or v==vmax:
        res = math.floor((res+v)/2)

        if res==0:
            res=1
        elif res==W-1:
            res -= 1
    return res

while 1:
    line = input()

 # On a la bonne position : on y va!
    if len(TX) == 1 and TX[0] != X:
        X=TX[0]
        print("%s %s" % (X,Y))
        continue
    if len(TY) == 1 and TY[0] != Y:
        Y=TY[0]
        print("%s %s" % (X,Y))
        continue
 # On réduit les intervalles de possibilité
    if line == 'SAME':
        if X != X0:
            TX = [math.floor((X+X0)/2)]
        else:
            TY = [math.floor((Y+Y0)/2)]
    elif line == 'WARMER':
        set_warmer(X0,X,Y0,Y)
    elif line == 'COLDER':
        set_warmer(X,X0,Y,Y0)

 # On mets à jour les positions de l'état précédent
    X0 = X
    Y0 = Y

 # Calcul des nouvelles positions
    if len(TX)>1:
        X = get_next(X, TX, W-1)
    else:
        Y = get_next(Y, TY, H-1)

 # On envoie les positions de Batman
    print("%s %s" % (X,Y))
