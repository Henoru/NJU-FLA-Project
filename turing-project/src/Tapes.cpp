#include "Tapes.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

Tape::Tape(TapeChar empty):empty(empty),index(0){
    succ.push_back(empty);
}
void Tape::load(std::string_view input){
    pred.clear(),succ.clear();
    for(auto& c:input)
        succ.push_back(c);
    if(succ.empty())
        succ.push_back(empty);
}
TapeChar Tape::pointTo(){
    return  succ.front();
}
void Tape::move(TapeChar target,Direction direct){
    succ[0]=target;
    switch (direct) {
        case Left:
            index-=1;
            if(!pred.empty()){
                succ.push_front(pred.back());
                pred.pop_back();
            }
            else succ.emplace_front(empty);
            break;
        case Right:
            index+=1;
            pred.push_back(succ.front());
            succ.pop_front();
            if(succ.empty())
                succ.emplace_back(empty);
            break;
        case Stay: break;
    }
}
void Tape::clean_buf(){
    while(!pred.empty()&&pred.front()==empty)
        pred.pop_front();
    while(succ.size()>1&&succ.back()==empty)
        succ.pop_back();
}
void Tape::clear(){
    pred.clear();
    succ.clear();
    succ.push_back(empty);
}
void Tape::ID(std::string& L,int32_t& ind,std::string& R){
    clean_buf();
    for(auto &c:pred)
        L+=c;
    for(auto &c:succ)
        R+=c; 
    ind=index;
}
std::string Tape::to_str(){
    std::string ans="";
    ID(ans,index,ans);
    return ans;
}
Tapes::Tapes(uint32_t tape_num,TapeChar empty):tape_num(tape_num){
    tapes.clear();
    for(uint32_t i=0;i<tape_num;i++)
        tapes.emplace_back(empty);
}
void Tapes::move(Move move){
    for(uint32_t i=0;i<tape_num;i++)
        tapes[i].move(move.target[i],static_cast<Direction>(move.direct[i]));
}
std::string Tapes::result(){
    return tapes[0].to_str();
}
TapesChar Tapes::pointTo(){
    TapesChar res="";
    for(auto& x:tapes)
        res+=x.pointTo();
    return res;
}
uint32_t Tapes::tapeNum(){return tape_num;}
void Tapes::load(std::string_view input){
    tapes[0].load(input);
} 
void Tapes::clear(){
    for(auto &x:tapes)
        x.clear();
}
std::string Tapes::ID(){
    uint32_t wid=5+std::to_string(tape_num).length();
    std::string res;
    for(uint32_t i=0;i<tape_num;i++){
        std::string Index,Tape,Head;
        Index="Index",Tape="Tape",Head="Head";
        Index+=std::to_string(i);
        Tape+=std::to_string(i);
        Head+=std::to_string(i);
        Index+=std::string(wid-Index.length()+1,' ');
        Index+=": ";
        Tape+=std::string(wid-Tape.length()+1,' ');
        Tape+=": ";
        Head+=std::string(wid-Head.length()+1,' ');
        Head+=": ";
        std::string L="",R="";
        int32_t index=0;
        tapes[i].ID(L,index,R);
        int32_t hd=index-L.length();
        L+=R;
        for(auto& c:L){
            std::string num=std::to_string(abs(hd));
            Tape+=c;
            Index+=num;
            Head+=(hd==index)?'^':' ';
            Index+=' ';
            Tape+=std::string(num.length(),' ');
            Head+=std::string(num.length(),' ');
            hd++;
        }
        res.append(Index).append("\n").append(Tape).append("\n").append(Head).append("\n");
    }
    return res;
}