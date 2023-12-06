#ifndef TM_TAPES
#define TM_TAPES
#include <cstdint>
#include <deque>
#include <string>
#include <vector>
enum Direction{
    Left='l',
    Stay='*',
    Right='r'
};
typedef std::string Directions;
typedef char        TapeChar;
typedef std::string TapesChar;
struct Move{
    TapesChar target;
    Directions direct;
    Move(TapesChar target,Directions direct):target(target),direct(direct){}
};
class Tape{
    private :
        int32_t index;
        TapeChar empty;
        std::deque<TapeChar> pred,succ;
        void clean_buf();
    public: 
        Tape(TapeChar empty);
        void load(std::string_view input);
        TapeChar pointTo();
        void move(TapeChar target,Direction direct);
        void clear();
        TapeChar getEmpty(); 
        std::string to_str();
        void ID(std::string& L,int32_t& index,std::string& R);
};
class Tapes{
    private:
        uint32_t tape_num;
        std::vector<Tape> tapes;
    public:
        Tapes(uint32_t tape_num,TapeChar empty);
        void move(Move move);
        TapesChar pointTo();
        uint32_t tapeNum();
        void load(std::string_view input);
        void clear();
        std::string result();
        std::string ID();
};
#endif