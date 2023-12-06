#ifndef TM_TRANSFORM
#define TM_TRANSFORM
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include "Tapes.hpp"
typedef std::string State;
typedef std::pair<uint32_t,Move> TransTarget;
class TransformRules{
    private:
        struct node{
            node* ch[256]={};
            TransTarget* tar{nullptr};
            ~node(){
                for(int i=0;i<256;i++)
                    delete ch[i];
                delete tar;
            }
        };
        node root;
        std::optional<TransTarget> dfs(node* cur,uint32_t ind,std::string_view str);
    public:
        bool add(std::string_view mode,uint32_t newState,Move move);
        std::optional<TransTarget> match(std::string_view str);
};

#endif