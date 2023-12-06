#ifndef TM_EXCEPTION
#define TM_EXCEPTION

#include <cstdint>
#include <exception>
#include <string>
extern bool verbose;

class tmSyntaError:std::exception {
    private: 
        std::string line;
        uint32_t ind,Line;
        std::string res;
    public:
        tmSyntaError(std::string line,uint32_t Line,uint32_t ind);
        const char* what();
};
#endif