project(TuringEmulator)
cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/turing-project/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project CPP_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project/src CPP_SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(turing ${CPP_SRC})

