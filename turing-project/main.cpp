#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>
#include "TM.hpp"
#include "parser.hpp"
#include "error.hpp"
using namespace std;
void help();
bool verbose=false;
TM tm;
std::string tmFileName,inputString;
vector<Error> errorList;
void help(){
    cerr << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
}
void move(){

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
    tmFileName=argv[ind],inputString=argv[ind+1];
    ifstream tmFile(tmFileName,ios::in);
    if(!tmFile.is_open()){
        cerr << "illegal input file "+tmFileName+" \n";
        return 0;
    }
    else{
        if(parse(tmFile,tm)){
            if(tm.load(inputString))
                move();
            else
                for(auto &x:errorList)
                    cerr << x.toPrint(verbose);
        }
        else 
            for(auto &x:errorList)
                    cerr << x.toPrint(verbose);
    }
    return 0;
}