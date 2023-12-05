#include "Tapes.hpp"
#include <cstdint>
#include <string_view>

Tape::Tape(TapeChar empty):empty(empty){
    index=0,succ.push_back(empty);
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
            pred.push_back(succ.front());
            succ.pop_front();
            if(succ.empty())
                succ.emplace_back(empty);
            break;
        case Right:
            index+=1;
            if(!pred.empty()){
                succ.push_front(pred.back());
                pred.pop_back();
            }
            else succ.emplace_front(empty);
            break;
        case Stay: break;
    }
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