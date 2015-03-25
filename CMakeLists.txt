project(spmie)
cmake_minimum_required(VERSION 2.8)

enable_language(Fortran)

set(Fortran_FLAGS "-lgfortran")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${Fortran_FLAGS}")

aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} "src/nequick.f" "src/slqu.f")

set(PROJECT_BINARY_DIR ./build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
