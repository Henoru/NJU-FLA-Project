#ifndef TM_EXCEPTION
#define TM_EXCEPTION
#include <string>
class Error{
    public:
         virtual std::string toPrint(bool verbose=false);
};

#endif