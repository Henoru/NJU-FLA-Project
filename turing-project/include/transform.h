#ifndef TM_TRANSFORM
#define TM_TRANSFORM
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include "Tapes.h"
typedef std::string State;
typedef std::pair<uint32_t,Move> TransTarget;
class TransformRules{
    private:
        struct node{
            node* ch[256]={};
            uint32_t idx;
            TransTarget* tar{nullptr};
            node(uint32_t idx):idx(idx){}
            ~node(){
                for(int i=0;i<256;i++)
                    delete ch[i];
                delete tar;
            }
        };
        uint32_t cnt;
        node root;
    public:
        TransformRules():cnt(0),root(0){}
        bool add(std::string_view mode,uint32_t newState,Move move);
        std::optional<TransTarget> match(std::string_view str);
};

#endif