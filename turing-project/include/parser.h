#ifndef TM_PARSER
#define TM_PARSER

#include "TM.h"
#include "Tapes.h"
#include "exception.h"
#include <fstream>
#include <optional>

std::optional<TM> parser(const char* path);
#endif