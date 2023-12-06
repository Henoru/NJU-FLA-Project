#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include "TM.h"
#include "parser.h"
using namespace std;
void help();
bool verbose=false;
std::optional<TM> tm;
void help(){
    cerr << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
}
int main(int argc, char* argv[]){
    if(argc==1){
        help();
        return 0;
    }
    int ind=1;
    for(;ind<argc;ind++){
        if (!strcmp(argv[ind],"-v") || !strcmp(argv[ind],"--verbose"))
            verbose=true; 
        else if (!strcmp(argv[ind], "--help") || !strcmp(argv[ind], "-h") ){
            help();
            return 0;    
        }
        else break;
    }
    if(ind+2!=argc){
        cerr << "Need <tm> <input> but "<< argc-ind << " arguments are provided.\n";
        return 0;
    }
    tm=parser(argv[ind]);
    if(!tm.has_value())exit(-1);
    tm->init();
    if(!tm->loadInput(argv[ind+1])){
        std::cerr << "illegal input string\n";
        return -1;
    }
    else if(verbose){
        std::cout << "Input: "<< argv[ind+1] << std::endl <<
        "==================== RUN ====================\n";
        std::cout << tm->ID();
        std::cout<< "---------------------------------------------\n";
        while(tm->move()){
            std::cout<< tm->ID();
            std::cout<< "---------------------------------------------\n";
        }
        std::cout<< tm->result(true);
        std::cout<< "==================== END ====================\n";
    }else{
        tm->run();
        std::cout << tm->result(false);
    }
    return 0;
}