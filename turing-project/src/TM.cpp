#include "TM.hpp"
#include "Tapes.hpp"
#include "transform.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
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
    Acc+=std::string(wid-Acc.length(),' ');
    Step.append(": ").append(std::to_string(step));
    State.append(": ").append(numToState[curState]);
    Acc.append(": ").append(accept?"Yes":"No");
    return res.append(Step).append("\n").append(State).append("\n").append(Acc).append("\n").append(tapes->ID());
}