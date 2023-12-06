#Q = {0,a,b,ret,cpy,bak,acc,ilg,emp,inv,i,il,ill,lle,leg,ega,gal,inr}

#S = {a,b}

#G = {a,b,c,_,X,i,l,e,g,i,n,p,u,t}

#q0 = 0

#B = _

#F = {acc}

#N = 3

;0: Start move a to Tape 1 b To Tape 2
0   a__ Xa_ rr* a
a   a__ _a_ rr* a
a   b__ __b r*r b
b   b__ __b r*r b 
b   ___ ___ lll ret
ret _** _** l** ret
ret Xa* Xa* *l* ret
ret X_b X_b **l ret
ret X__ ___ *rr cpy
;illegal input
0 b** b** *** emp ; no a
0 _** _** *** emp ; empty
a _** _** *** inv ; no b
b a** a** *** inv ; a between b

;1: Start copy b to Tape0
cpy _ab cab r*r cpy
cpy _a_ _a_ **l bak
bak _ab _ab **l bak
bak _a_ _a_ *rr cpy
cpy __b __b *** acc

;2: write illegal
emp *** i** r** i
i   *** l** r** il
il  *** l** r** ill
ill *** e** r** lle
lle *** g** r** leg
leg *** a** r** ega
ega *** l** r** gal
gal a** _** r** gal
gal b** _** r** gal

inv a** _** r** inv
inv b** _** r** inv
inv _** _** l** inr ; return to X__
inr _** _** l** inr 
inr X** _** *** emp
