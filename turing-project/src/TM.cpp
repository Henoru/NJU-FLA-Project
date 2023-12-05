#include "TM.hpp"
#include "Tapes.hpp"
#include "transform.hpp"
#include <cstdint>
#include <optional>
#include <string_view>

uint32_t TM::tapeNum(){return tapes->tapeNum();}
bool TM::loadInput(std::string_view input){
    for(auto& c:input)
        if(!legalInputChar[c])
            return false;
    tapes->load(input);
    return true;
}
bool TM::move(){
    std::string&& head=tapes->pointTo();
    std::optional<TransTarget> tar=trans[curState].match(head);
    if(tar.has_value()){
        curState=tar->first;
        tapes->move(tar->second);
        if(acceptState.count(curState))
            accept=true;
    }
    else return false;
}
void TM::init(){
    curState=initState;
    accept=false;
}
void TM::run(){
    while(move());
}
