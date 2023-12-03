#ifndef TMLIB_TM
#define TMLIB_TM

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include "error.hpp"
extern bool verbose;
extern std::vector<Error> errorList;
typedef char        TapeChar;
typedef std::string TapesChar;
enum Direction{
    Left=-1,
    Right=1
};
typedef  std::pair<TapesChar,Direction> Move;
class Tapes{
    private :
        int32_t index;
        std::vector<TapesChar> pred,succ;
    public: 
        Tapes():index(0),pred(),succ(){}
        void load(std::string input);
        TapesChar pointTo();
        void move(Move move);
        void clean();
};
enum MoveState{
    NotHalt =0,
    Halt
};
class TM{
    private: 
        uint32_t tape_num;
        Tapes tapes;
        std::set<TapeChar> inputChar,tapeChar;
    public:
        TM()=default;
        TM(TM&&)=default;
        TM(const TM&)=default;
        TM(uint32_t tape_num,std::set<TapeChar>&& inputChar,std::set<TapesChar>&& tapeChar):
            tape_num(tape_num),
            inputChar(inputChar){};
        bool load(std::string_view);
        MoveState move(Move move);
        std::string toPrint();
        TM& operator=(TM &&)=default;
        TM& operator=(const TM&)=default;
};
#endif