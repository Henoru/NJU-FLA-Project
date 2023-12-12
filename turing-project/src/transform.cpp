#include "transform.h"
#include "TM.h"
#include "Tapes.h"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

bool TransformRules::add(std::string_view mode,uint32_t newState,Move move){
    node* cur=&root;
    for(auto& x:mode){
        if(cur->ch[x]==nullptr)
           cur->ch[x]=new node;
        cur=cur->ch[x];
    }
    if(cur->tar==nullptr) cur->tar=new TransTarget(newState,move);
    else return false;
    return true;
}
std::optional<TransTarget> TransformRules::match(std::string_view str){
    std::optional<TransTarget> res=dfs(&root,0,str);
    if(!res.has_value()) 
        return res;
    for(uint32_t i=0;i<str.length();i++)
        if(res.value().second.target[i]=='*')
            res.value().second.target[i]=str[i];
    return res;
}
std::optional<TransTarget> TransformRules::dfs(node* cur,uint32_t ind,std::string_view str){
    if(ind==str.length())
        return ((cur->tar)!=nullptr)?(std::make_optional(*(cur->tar))):(std::nullopt);
    std::optional<TransTarget> res;
    if(cur->ch[str[ind]]!=nullptr)
        res=dfs(cur->ch[str[ind]],ind+1,str);
    if(res.has_value())
        return res;
    if(str[ind]!='_'&&cur->ch['*']!=nullptr)
        res=dfs(cur->ch['*'],ind+1,str);
    return res;
}