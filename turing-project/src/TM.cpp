#include "TM.h"
#include "Tapes.h"
#include "transform.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

uint32_t TM::tapeNum(){return tapes->tapeNum();}
uint32_t TM::loadInput(std::string_view input){
    for(uint32_t ind=0;ind<input.length();ind++)
        if(!legalInputChar[input[ind]])
            return ind+1;
    tapes->load(input);
    return 0;
}
bool TM::move(){
    step++;
    std::string&& head=tapes->pointTo();
    std::optional<TransTarget> tar=trans[curState].match(head);
    if(tar.has_value()){
        curState=tar->first;
        tapes->move(tar->second);
        if(acceptState.count(curState))
            accept=true;
        return true;
    }
    else return false;
}
void TM::init(){
    curState=initState;
    accept=false;
    step=0;
    tapes->clear();
}
void TM::run(){
    while(move());
}
bool TM::run(std::string_view input){
    init();
    if(!loadInput(input)) return false;
    run();
    return true;
}
std::string TM::result(bool verbose){
    std::string res;
    if(!verbose){
        res=(accept)?("(ACCEPTED) "):("(UNACCEPTED) ");
        res+=tapes->result();
        return res;
    }
    res=(accept)?("ACCEPTED\n"):("UNACCEPTED\n");
    res+="Result: ";
    res+=tapes->result();
    res+='\n';
    return res;
}
std::string TM::ID(){
    std::string Step="Step",State="State",Acc="Acc";
    std::string res;
    uint32_t wid=5+std::to_string(tapeNum()).length();
    Step+=std::string(wid-Step.length()+1,' ');
    State+=std::string(wid-State.length()+1,' ');
    Acc+=std::string(wid-Acc.length()+1,' ');
    Step+=": "+std::to_string(step);
    State+=": "+numToState[curState];
    Acc+=": "+std::string((accept?"Yes":"No"));
    return Step+"\n"+State+"\n"+Acc+"\n"+tapes->ID();
}