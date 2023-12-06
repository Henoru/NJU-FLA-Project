#ifndef TM_EXCEPTION
#define TM_EXCEPTION

#include <cstdint>
#include <exception>
#include <string>
extern bool verbose;

class tmSyntaError:public std::exception {
    protected: 
        std::string line;
        uint32_t ind,Line;
        std::string res;
    public:
        tmSyntaError(std::string line,uint32_t Line,uint32_t ind);
        virtual const char* what() throw();
};
class tmTypedSyntaError:public tmSyntaError{
    protected:
        std::string type;
    public:
        tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind);
        tmTypedSyntaError(std::string line,uint32_t Line,uint32_t ind,std::string type);
        virtual const char* what()  throw();
};
#endif