#ifndef TM_ERROR
#define TM_ERROR
#include <string>
class Error{
    public:
         virtual std::string toPrint(bool verbose=false);
};

#endif