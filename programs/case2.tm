#Q = {0,acc,ret,mat,retT,retF,t,tr,tru,true,f,fa,fal,fals,false,clr}

#S = {a,b,c}

#G = {a,b,c,_,t,r,u,e,f,l,s}

#q0 = 0

#B = _

#F = {acc}

#N = 2

;0 move u in ucu to tape 1
0   a_ aa rr 0
0   b_ bb rr 0
0   c_ c_ rl ret

;when find c tape 1 index return to head

ret *a *a *l ret
ret *b *b *l ret
ret *_ *_ *r mat

;match u u
mat aa aa rr mat
mat bb bb rr mat
mat __ __ ll retT

; stupid ** if match mode
mat ab ab ll retF
mat ba ba ll retF
mat a_ a_ ll retF
mat b_ b_ ll retF
mat _a _a ll retF
mat _b _b ll retF

; retT and write True
retT a* a* l* retT 
retT b* b* l* retT 
retT c* c* l* retT 
retT _* _* r* acc
acc  ** ** ** t
t    ** t* r* tr
tr   ** r* r* tru 
tru  ** u* r* true
true ** e* r* clr

; retF and write False
retF a* a* l* retF
retF b* b* l* retF
retF c* c* l* retF 
retF _* _* r* f

f     ** f* r* fa
fa    ** l* r* fal
fal   ** a* r* fals
fals  ** s* r* false
false ** e* r* clr

;clear longer than answer
clr  a* _* r* clr
clr  b* _* r* clr
clr  c* _* r* clr