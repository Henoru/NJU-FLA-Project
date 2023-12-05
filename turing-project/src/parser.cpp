#include "parser.hpp"
#include "Tapes.hpp"
#include "transform.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
char invaildChar[]={' ',',',';','{','}','*','_'};
bool TM::setInitState(const State& state){
    auto it=stateTonum.find(state);
    if(it==stateTonum.end())
        return false;
    initState=it->second;
    return true;
}
bool TM::addInputChar(TapeChar c){
    for(auto& chr:invaildChar)
        if(c==chr) return false;
    legalInputChar[c]=true;
    return true;
}
bool TM::addTapeChar(TapeChar c){
    for(auto& chr:invaildChar)
        if(c==chr) return false;
    legalInputChar[c]=true;
            legalTapeChar[c]=true;
    return true;
}
bool TM::addState(const State& state){
    if(stateTonum.count(state))
        return false;
    stateTonum[state]=numOfState++;
    numToState.emplace_back(state);
    trans.emplace_back();
    return true;
}
bool TM::addAcceptState(const State& state){
    auto it=stateTonum.find(state);
    if(it==stateTonum.end())
        return false;
    acceptState.insert(it->second);
    return true;
}
bool TM::addRules(const State& oldState,const State& mode,const State& target,Directions direct,const State& newState){
    auto itOld=stateTonum.find(oldState);
    auto itNew=stateTonum.find(newState);
    if(itOld==stateTonum.end())
        return false;
    if(mode.length()!=tapeNum())
        return false;
    if(target.length()!=tapeNum())
        return false;
    for(uint32_t i=0;i<tapeNum();i++)
        if(mode[i]!='*' && target[i]=='*')
            return false;
    if(itNew==stateTonum.end())
        return false;

    return trans[itOld->second].add(mode,itNew->second,Move(target,direct));
}
bool TM::setTapeNum(uint32_t tape_num){
    if(tape_num<=0) throw;
    tapes=std::make_unique<Tapes>(tape_num,empty);
    return true;
}
bool TM::setEmpty(TapeChar c){
    if(c!='_') throw;
    empty=c;
    return true;
}
#define matchHead \
        while(ind<line.length()&&line[ind]==' ') ind++;\
        if(ind==line.length()) continue;\
        if(line[ind]==';') continue;\
        if(ind+head.length()>line.length()) throw;\
        for(int j=0;j<head.length();j++,ind++)\
            if(line[ind]!=head[j])\
                throw;\
        while(ind<line.length()&&line[ind]==' ') ind++;\
        if(ind==line.length()||line[ind]!='=') throw; \
        else ind++;
#define matchL \
    for(;ind<line.length();ind++)\
        if(line[ind]==' ') continue;\
        else if(line[ind]=='{') break;\
        else throw;\
    if(ind==line.length()) throw;
#define matchR \
    for(;ind<line.length();ind++)\
        if(line[ind]==' ') continue;\
        else if(line[ind]==';') break;\
        else throw;
#define between(c,x,y) (x<=c && c<=y)
bool charInState(char c){
    return ( between(c, 'a', 'b') ||
             between(c, 'A', 'B') ||
             between(c, '0', '9') ||
             c=='_');
}
#define match(str) match##str(file,Line);
TM::TM(std::istream& file){
    uint32_t state=0;
    uint32_t Line=0;
    std::string line;
    std::string head="#Q";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        matchL
        for(;ind<line.length();ind++){
            std::string item="";
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            while(ind<line.length()&&charInState(line[ind]))
                item+=line[ind++];
            if(ind==line.length()) throw;
            addState(item);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            if(line[ind]==',') continue;
            if(line[ind]=='}') continue;
        }
        matchR
    }
    head="#S";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        matchL
        for(;ind<line.length();ind++){
            std::string item="";
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            addInputChar(line[ind++]);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            if(line[ind]==',') continue;
            if(line[ind]=='}') continue;
        }
        matchR
    }
    head="#G";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        matchL
        for(;ind<line.length();ind++){
            std::string item="";
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            addTapeChar(line[ind++]);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            if(line[ind]==',') continue;
            if(line[ind]=='}') continue;
        }
        matchR
    }
    head="#q0";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        std::string item="";
        while(ind<line.length()&&line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        while(ind<line.length() && charInState(line[ind])) 
            item+=line[ind++];
        setInitState(item);
        matchR
    }
    head="#B";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        std::string item="";
        while(ind<line.length()&&line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        setEmpty(line[ind++]);
        matchR
    }
    head="#F";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        matchL
        for(;ind<line.length();ind++){
            std::string item="";
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            while(ind<line.length()&&charInState(line[ind]))
                item+=line[ind++];
            if(ind==line.length()) throw;
            addAcceptState(item);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw;
            if(line[ind]==',') continue;
            if(line[ind]=='}') continue;
        }
        matchR
    }
    head="#N";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        uint32_t item=0;
        while(ind<line.length()&&line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        while(ind<line.length() && between(line[ind], '0', '9')) 
            item=item*10+(line[ind++]-'0');
        setTapeNum(item);
        matchR
    }
    head="";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) continue;
        if(line[ind]==';') continue;
        State oldState="",newState="";
        std::string mode="",target="";
        Directions direct="";
        while(ind<line.length() && line[ind]!=' ')
            oldState+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        while(ind<line.length() && line[ind]!=' ')
            mode+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        while(ind<line.length() && line[ind]!=' ')
            target+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        while(ind<line.length() && line[ind]!=' ')
            direct+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw;
        matchR
    }
}