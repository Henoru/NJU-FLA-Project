#include "parser.h"
#include "TM.h"
#include "Tapes.h"
#include "exception.h"
#include "transform.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
char invaildInputChar[]=" ,;{}*_";
char invaildTapeChar[] =" ,;{}*";
bool TM::setInitState(const State& state){
    auto it=stateTonum.find(state);
    if(it==stateTonum.end())
        return false;
    initState=it->second;
    return true;
}
bool TM::addInputChar(TapeChar c){
    for(auto& chr:invaildInputChar)
        if(c==chr) return false;
    legalInputChar[c]=true;
    return true;
}
bool TM::addTapeChar(TapeChar c){
    for(auto& chr:invaildTapeChar)
        if(c==chr) return false;
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
uint32_t TM::addRules(const State& oldState,const State& mode,const State& target,Directions direct,const State& newState){
    auto itOld=stateTonum.find(oldState);
    auto itNew=stateTonum.find(newState);
    if(itOld==stateTonum.end())
        return 1;
    if(mode.length()!=tapeNum())
        return 2;
    for(auto &c:mode)
        if(c!='*'&&!legalTapeChar[c]) return 2;
    if(target.length()!=tapeNum())
        return 3;
    for(auto &c:target)
        if(c!='*'&&!legalTapeChar[c]) return 3;
    for(uint32_t i=0;i<tapeNum();i++)
        if(mode[i]!='*' && target[i]=='*')
            return 3;
    for(auto & c:direct)
        if(c!='l' && c!='r' && c!='*') return 4;
    if(itNew==stateTonum.end())
        return 5;
    return (trans[itOld->second].add(mode,itNew->second,Move(target,direct)))?0:2;
}
bool TM::setTapeNum(uint32_t tape_num){
    if(tape_num<=0) return false;
    tapes=std::make_unique<Tapes>(tape_num,empty);
    return true;
}
bool TM::setEmpty(TapeChar c){
    if(c!='_') return false;
    if(!legalTapeChar[c]) return false;
    empty=c;
    return true;
}
#define matchHead \
        while(ind<line.length()&&line[ind]==' ') ind++;\
        if(ind==line.length()) continue;\
        if(line[ind]==';') continue;\
        if(ind+head.length()>line.length()) throw  tmSyntaError(line,Line,ind);\
        for(int j=0;j<head.length();j++,ind++)\
            if(line[ind]!=head[j])\
                throw  tmSyntaError(line,Line,ind);\
        while(ind<line.length()&&line[ind]==' ') ind++;\
        if(ind==line.length()||line[ind]!='=') throw  tmSyntaError(line,Line,ind); \
        else ind++;
#define matchL \
    for(;ind<line.length();ind++)\
        if(line[ind]==' ') continue;\
        else if(line[ind]=='{') {ind++;break;}\
        else throw  tmSyntaError(line,Line,ind);\
    if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
#define matchR \
    for(;ind<line.length();ind++)\
        if(line[ind]==' ') continue;\
        else if(line[ind]==';') break;\
        else throw  tmSyntaError(line,Line,ind);\
    break;
#define between(c,x,y) (x<=c && c<=y)
bool charInState(char c){
    return ( between(c, 'a', 'z') ||
             between(c, 'A', 'Z') ||
             between(c, '0', '9') ||
             c=='_');
}
#define match(str) match##str(file,Line);
#define matchBlank \
while(ind<line.length()&&line[ind]==' ') ind++;\
if(ind==line.length()) throw  tmSyntaError(line,Line,ind);

#define matchNextItem \
if(line[ind]==',') continue;\
if(line[ind]=='}') {ind++;break;}\
else throw tmSyntaError(line,Line,ind);
#define matchStateItem \
std::string item;\
uint32_t token_head=ind;\
while(ind<line.length()&&charInState(line[ind]))\
    item+=line[ind++];

#define mathchCharItem \
char item=line[ind];\
uint32_t token_head=ind++;
#define matchSingleState(fun) \
for(;std::getline(file,line);Line++){\
        uint32_t ind=0;\
        matchHead\
        matchBlank\
        matchStateItem\
        if(!fun(item)) throw tmSyntaError(line,Line,token_head);\
        matchR\
    }
#define matchMultiState(fun) \
for(;std::getline(file,line);Line++){\
        uint32_t ind=0;\
        matchHead\
        matchL\
        for(;ind<line.length();ind++){\
            matchBlank\
            matchStateItem\
            if(!fun(item)) throw  tmSyntaError(line,Line,token_head);\
            matchBlank\
            matchNextItem\
        }\
        matchR\
    }
#define matchSigleChar(fun) \
for(;std::getline(file,line);Line++){\
        uint32_t ind=0;\
        matchHead\
        matchBlank\
        mathchCharItem\
        if(!fun(item)) throw  tmSyntaError(line,Line,token_head);\
        matchR\
    }
#define matchMultiChar(fun) \
for(;std::getline(file,line);Line++){\
        uint32_t ind=0;\
        matchHead\
        matchL\
        for(;ind<line.length();ind++){\
            matchBlank\
            mathchCharItem\
            if(!fun(item)) throw  tmSyntaError(line,Line,token_head);\
            matchBlank\
            matchNextItem\
        }\
        matchR\
    }
TM::TM(std::istream& file):
        tapes(nullptr),numOfState(0),
        initState(0),curState(0),empty('_')
{
    memset(legalInputChar,0,sizeof(legalTapeChar));
    memset(legalTapeChar, 0, sizeof(legalTapeChar));
    uint32_t state=0;
    uint32_t Line=0;
    std::string line;
    std::string head="#Q";
    matchMultiState(addState)
    head="#S";
    matchMultiChar(addInputChar)
    head="#G";
    matchMultiChar(addTapeChar)
    // Input <= Tape
    for(uint32_t c=0;c<256;c++)
        if(legalInputChar[c]&& !legalTapeChar[c]){
            std::string temp;
            temp="Input symbol \'";
            temp+=std::string(1,char(c));
            temp+="\' should be included in the set of tape symbols\n";
            throw tmTypedSyntaError(line,Line,0,temp);
        }      
    head="#q0";
    matchSingleState(setInitState)
    head="#B";
    matchSigleChar(setEmpty)
    head="#F";
    matchMultiState(addAcceptState)
    head="#N";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        uint32_t item=0;
        matchBlank
        uint32_t token_head=ind;
        while(ind<line.length() && between(line[ind], '0', '9')) 
            item=item*10+(line[ind++]-'0');
        if(!setTapeNum(item)) throw  tmSyntaError(line,Line,token_head);
        matchR
    }
    head="";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) continue;
        if(line[ind]==';') continue;
        do{
            State oldState="",newState="";
            std::string mode="",target="";
            Directions direct="";
            uint32_t token_head[5],cnt=0;
            token_head[cnt++]=ind;
            while(ind<line.length() && line[ind]!=' ')
                oldState+=line[ind++];
            matchBlank
            token_head[cnt++]=ind;
            while(ind<line.length() && line[ind]!=' ')
                mode+=line[ind++];
            matchBlank
            token_head[cnt++]=ind;
            while(ind<line.length() && line[ind]!=' ')
                target+=line[ind++];
            matchBlank
            token_head[cnt++]=ind;
            while(ind<line.length() && line[ind]!=' ')
                direct+=line[ind++];
            matchBlank
            token_head[cnt++]=ind;
            while(ind<line.length() && line[ind]!=' ')
                newState+=line[ind++];
            uint32_t ret_code=addRules(oldState, mode, target, direct, newState);
            if(ret_code) 
                throw  tmSyntaError(line,Line,token_head[ret_code-1]);
            matchR
        }while(0);
    }
}

std::optional<TM> parser(const char* path){
    std::ifstream file;
    file.open(path,std::ios_base::in);
    if(!file.is_open()) {
        std::cerr <<  "TM file " << path << " can't be open\n";
        return  std::nullopt;
    }
    try {
        TM tm(file);
        file.close();
        return std::make_optional(std::move(tm));
    } catch (tmSyntaError s) {
        if(verbose) std::cerr << s.what();
        else std::cerr << "syntax error\n";
        file.close();
        return std::nullopt;
    } 
}