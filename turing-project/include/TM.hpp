#ifndef TMLIB_TM
#define TMLIB_TM

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include "exception.hpp"
#include "transform.hpp"
#include "Tapes.hpp"
class TM{
    private: 
        std::unique_ptr<Tapes> tapes;
        uint32_t numOfState;
        uint32_t initState,curState;
        bool legalInputChar[256]={0};
        bool legalTapeChar[256]={0};
        bool accept;
        uint32_t step;
        TapeChar empty;
        std::vector<State> numToState;
        std::set<uint32_t> acceptState;
        std::map<State,uint32_t> stateTonum;
        std::vector<TransformRules> trans;
        /*
            functions for build
        */
        bool setTapeNum(uint32_t tape_num);
        bool setInitState(const State& state);
        bool addInputChar(TapeChar c);
        bool addTapeChar(TapeChar c);
        bool setEmpty(TapeChar c);
        bool addState(const State& state);
        bool addAcceptState(const State& state);
        /*
            0: accepte
            1-5: ret argument illegal 
            eg. 1 means illegal oldState
        */
        uint32_t addRules(const State& oldState,const State& mode,const State& target,Directions direct,const State& newState);
     public:
        TM(std::istream& file);
        TM(TM&&)=default;
        uint32_t tapeNum();
        void initTapes();
        bool loadInput(std::string_view input);
        bool move();
        void run();
        bool run(std::string_view input);
        void init();
        std::string ID();
        std::string result(bool verbose=false);
        TM& operator=(TM &&)=default;
};
#endif