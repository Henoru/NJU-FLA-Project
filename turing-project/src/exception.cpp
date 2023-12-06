#include "exception.hpp"
#include <string>

const char* tmSyntaError::what(){
    std::string num=std::to_string(Line);
    res="SyntaxError\nLine ";
    res+=num+":";
    res+=line+"\n"+std::string(6+num.length()+ind,' ')+"^\n";
    return res.data();
}
tmSyntaError::tmSyntaError(std::string line,uint32_t Line,uint32_t ind):Line(Line),line(line),ind(ind){}