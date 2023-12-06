#include "exception.hpp"
#include <cstring>
#include <iostream>
#include <string>

const char* tmSyntaError::what() throw(){
    std::string num=std::to_string(Line);
    res="SyntaxError\nLine ";
    res+=num+":";
    res+=line+"\n"+std::string(6+num.length()+ind,' ')+"^\n";
    return res.c_str();
}
tmSyntaError::tmSyntaError(std::string line,uint32_t Line,uint32_t ind):Line(Line),line(){}
tmTypedSyntaError::tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind):
    tmTypedSyntaError(line,Line,ind,""){}
tmTypedSyntaError::tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind,std::string type):
    tmSyntaError(line,Line,ind),type(type){}

const char* tmTypedSyntaError::what() throw(){
    std::string num=std::to_string(Line);
    res="SyntaxError";
    res+=num+":";
    res+=line+"\n"+std::string(6+num.length()+ind,' ')+"^\n"+type+"\n";
    return res.c_str();
}