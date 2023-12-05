#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include "TM.hpp"
#include "parser.hpp"
using namespace std;
void help();
bool verbose=false;
std::unique_ptr<TM> tm;
std::string tmFileName,inputString;
vector<Error> errorList;
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
    ifstream file(argv[ind],ios::in); 
    try{
        tm=std::make_unique<TM>(file);
        tm->loadInput(argv[ind+1]);
    }
    catch(...){

    }
    tm->init();
    tm->run();
    return 0;
}