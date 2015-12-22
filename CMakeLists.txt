project(spmie)
cmake_minimum_required(VERSION 2.8)
set(LIB_RSIM_NAME "rsim")

enable_language(Fortran)

set(Fortran_FLAGS "-lgfortran")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${Fortran_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -std=c++11 -U__STRICT_ANSI__")

aux_source_directory(src EXE_SRC_LIST)

# librsim sources
aux_source_directory(src/librsim/astro LIBRSIM_SRC_LIST)
aux_source_directory(src/librsim/nequick LIBRSIM_SRC_LIST)
aux_source_directory(src/librsim/astro/sgp_sdp_model LIBRSIM_SRC_LIST)

add_library(${LIB_RSIM_NAME} SHARED ${LIBRSIM_SRC_LIST} "src/librsim/nequick/nequick.f" "src/librsim/nequick/slqu.f" src/librsim/astro/TLEReader.cpp src/librsim/astro/TLEReader.h src/librsim/ImitationDriver.cpp src/librsim/ImitationDriver.h src/librsim/SightReport.cpp src/librsim/SightReport.h src/librsim/SightObject.cpp src/librsim/SightObject.h)

add_executable(${PROJECT_NAME} ${EXE_SRC_LIST})

file(GLOB DATA_FILES data *)
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(
    ${PROJECT_NAME}
    ${LIB_RSIM_NAME}
)

set(PROJECT_BINARY_DIR ./build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

enable_testing()
