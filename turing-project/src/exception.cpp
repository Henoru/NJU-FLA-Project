#include "exception.h"
#include <cstring>
#include <iostream>
#include <string>

const char* tmSyntaError::what()const throw(){
    return res.c_str();
}
tmSyntaError::tmSyntaError(std::string line,uint32_t Line,uint32_t ind):Line(Line),line(){
    std::string num=std::to_string(Line);
    res="SyntaxError\nLine ";
    res+=num+":";
    res+=line+"\n"+std::string(6+num.length()+ind,' ')+"^\n";
}
tmTypedSyntaError::tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind):
    tmTypedSyntaError(line,Line,ind,""){}
tmTypedSyntaError::tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind,std::string type):
    tmSyntaError(line,Line,ind),type(type){
        std::string num=std::to_string(Line);
        res="SyntaxError\nLine ";
        res+=num+":";
        res+=line+"\n"+std::string(6+num.length()+ind,' ')+"^\n"+type+"\n";
    }

const char* tmTypedSyntaError::what()const throw(){
    
    return res.c_str();
}