#ifndef TM_PARSER
#define TM_PARSER

#include "TM.hpp"
#include "error.hpp"
#include <fstream>
#include <optional>
extern bool verbose;
extern std::vector<Error>  errorList;
std::optional<TM> parse(const std::ifstream& file,TM & tm);
#endif