#include "parser.hpp"
#include "TM.hpp"
#include "Tapes.hpp"
#include "exception.hpp"
#include "transform.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
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
    if(tape_num<=0) return false;
    tapes=std::make_unique<Tapes>(tape_num,empty);
    return true;
}
bool TM::setEmpty(TapeChar c){
    if(c!='_') return false;
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
        else throw  tmSyntaError(line,Line,ind);
#define between(c,x,y) (x<=c && c<=y)
bool charInState(char c){
    return ( between(c, 'a', 'z') ||
             between(c, 'A', 'Z') ||
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
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&charInState(line[ind]))
                item+=line[ind++];
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(!addState(item)) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(line[ind]==',') continue;
            if(line[ind]=='}') break;
        }
        ind++;
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
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(!addInputChar(line[ind++])) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(line[ind]==',') continue;
            if(line[ind]=='}') break;
        }
        ind++;
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
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(!addTapeChar(line[ind++])) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(line[ind]==',') continue;
            if(line[ind]=='}') break;;
        }
        ind++;
        matchR
    }
    head="#q0";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        std::string item="";
        while(ind<line.length()&&line[ind]==' ') ind++;
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
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
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
        if(!setEmpty(line[ind++])) throw  tmSyntaError(line,Line,ind);
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
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&charInState(line[ind]))
                item+=line[ind++];
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(!addAcceptState(item)) throw  tmSyntaError(line,Line,ind);
            while(ind<line.length()&&line[ind]==' ') ind++;
            if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
            if(line[ind]==',') continue;
            if(line[ind]=='}') break;
        }
        ind++;
        matchR
    }
    head="#N";
    for(;std::getline(file,line);Line++){
        uint32_t ind=0;
        matchHead
        uint32_t item=0;
        while(ind<line.length()&&line[ind]==' ') ind++;
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
        while(ind<line.length() && between(line[ind], '0', '9')) 
            item=item*10+(line[ind++]-'0');
        if(!setTapeNum(item)) throw  tmSyntaError(line,Line,ind);
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
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
        while(ind<line.length() && line[ind]!=' ')
            mode+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
        while(ind<line.length() && line[ind]!=' ')
            target+=line[ind++];
        while(ind<line.length() && line[ind]==' ') ind++;
        if(ind==line.length()) throw  tmSyntaError(line,Line,ind);
        while(ind<line.length() && line[ind]!=' ')
            direct+=line[ind++];
        if(!addRules(oldState, mode, target, direct, newState)) throw  tmSyntaError(line,Line,ind);
        matchR
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