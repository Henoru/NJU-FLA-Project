#Q = {0,a,b,ret,cpy,bak,acc,ilg,emp,inv,i,il,ill,lle,leg,ega,gal,inr,al_,l_i,_in,inp,npu,put,invret}

#S = {a,b}

#G = {a,b,c,_,X,I,l,e,g,n,p,u,t}

#q0 = 0

#B = _

#F = {acc}

#N = 3

;0: Start move a to Tape 1 and move b to Tape 2
0   a__ Xa_ rr* a   ; move a to tape 1
a   a__ _a_ rr* a
a   b__ __b r*r b   ; move b to tape 2
b   b__ __b r*r b 
b   ___ ___ lll ret ; clear tape 0
ret _** _** l** ret
ret Xa* Xa* *l* ret
ret X_b X_b **l ret
ret X__ ___ *rr cpy ; cpy: start to cpy b for a times

;illegal input 
0 b__ X__ *** inv ; no a
0 ___ ___ *** emp ; empty input
a ___ ___ *** inv ; no b
b a__ a__ *** inv ; a between b

;1: Start copy b for a times to tape 0
cpy _ab cab r*r cpy
cpy _a_ _a_ **l bak
bak _ab _ab **l bak
bak _a_ _a_ *rr cpy
cpy __b __b *** acc

;2: write Illegal_Input
emp ___ I__ r** i
i   ___ l__ r** il
il  ___ l__ r** ill
ill ___ e__ r** lle
lle ___ g__ r** leg
leg ___ a__ r** ega
ega ___ l__ r** gal
gal ___ ___ r** al_
al_ ___ I__ r** l_i
l_i ___ n__ r** _in 
_in ___ p__ r** inp
inp ___ u__ r** npu
npu ___ t__ r** put

inv a__ ___ r** inv
inv b__ ___ r** inv
inv ___ ___ l** invret
invret ___ ___ l** invret
invret a__ ___ l** invret
invret b__ ___ l** invret
invret X__ ___ *** emp
