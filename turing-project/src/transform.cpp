#include "transform.h"
#include "TM.h"
#include "Tapes.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

bool TransformRules::add(std::string_view mode,uint32_t newState,Move move){
    node* cur=&root;
    for(auto& x:mode){
        if(cur->ch[x]==nullptr)
           cur->ch[x]=new node(cnt);
        cur=cur->ch[x];
    }
    if(cur->tar==nullptr) cur->tar=new TransTarget(newState,move);
    else return true;
    cnt++;
    return true;
}
std::optional<TransTarget> TransformRules::match(std::string_view str){
    struct nodeCmp{
        node* v;
        uint32_t depth;
        bool operator>(const nodeCmp& y)const{
            return this->v->idx>y.v->idx;
        }
        nodeCmp(node* x):v(x),depth(0){}
        nodeCmp(node& x):v(&x),depth(0){}
        nodeCmp(node* x,uint32_t depth):v(x),depth(depth){}
        nodeCmp(node& x,uint32_t depth):v(&x),depth(depth){}
    };
    std::priority_queue<nodeCmp,std::vector<nodeCmp>,std::greater<nodeCmp>> queue;
    queue.emplace(root);
    while(!queue.empty()){
        nodeCmp cur=queue.top();
        queue.pop();
        if(cur.depth==str.size()){
            TransTarget tar=*cur.v->tar;
            for(uint32_t i=0;i<str.length();i++)
                if(tar.second.target[i]=='*')
                    tar.second.target[i]=str[i];
            return std::make_optional(tar);
        }
        if(cur.v->ch[str[cur.depth]]!=nullptr) 
            queue.emplace(cur.v->ch[str[cur.depth]],cur.depth+1);
        if(str[cur.depth]!='_'&&cur.v->ch['*']!=nullptr) 
            queue.emplace(cur.v->ch['*'],cur.depth+1);
    }
    return std::nullopt;
}
