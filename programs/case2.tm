#Q = {0,1,mat,ret1,retT,retF,clrF,t,tr,tru,true,f,fa,fal,fals,false,acc,unacc}

#S = {a,b,c}

#G = {a,b,c,_,t,r,u,e,f,l,s,X}

#q0 = 0

#B = _

#F = {acc}

#N = 2

; move u to tape 1

0    a_ Xa rr 1
0    b_ Xb rr 1
0    c_ X_ r* mat
1    a_ _a rr 1
1    b_ _b rr 1
1    c_ c_ *l ret1 ;return to tape1 head
ret1 ca ca *l ret1
ret1 cb cb *l ret1
ret1 c_ __ rr mat

;invaid
0 __ _c ** f
1 __ _c ** retF

; mat u & u
mat aa __ rr mat
mat bb __ rr mat
mat __ _c l* retT

mat ab _c r* clrF
mat ba _c r* clrF
mat a_ _c r* clrF
mat b_ _c r* clrF
mat c* _c r* clrF
mat c_ _c r* clrF

mat _* _c l* retF

; clear tape 1
clrF ** _* r* clrF
clrF _* _* l* retF

;retT retF
retT _* _* l* retT
retT X* _* ** t
retF _* _* l* retF
retF X* _* ** f

t    _* t* r* tr
tr   _* r* r* tru
tru  _* u* r* true
true _* e* ** acc  

f     _* f* r* fa
fa    _* a* r* fal
fal   _* l* r* fals
fals  _* s* r* false
false _* e* r* unacc
