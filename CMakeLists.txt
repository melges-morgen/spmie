project(spmie)
cmake_minimum_required(VERSION 2.8)

enable_language(Fortran)

set(Fortran_FLAGS "-lgfortran")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${Fortran_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -std=c++11 -U__STRICT_ANSI__")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

aux_source_directory(src SRC_LIST)
aux_source_directory(src/sgp_sdp_model SRC_LIST)
aux_source_directory(src/sgp_sdp_model SGP_MODEL)
add_executable(${PROJECT_NAME} ${SRC_LIST} "src/nequick.f" "src/slqu.f")

set(PROJECT_BINARY_DIR ./build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
