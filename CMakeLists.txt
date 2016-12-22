project(spmie)
cmake_minimum_required(VERSION 2.8)
set(LIB_RSIM_NAME "rsim")

enable_language(Fortran)

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(Fortran_FLAGS "-lgfortran")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${Fortran_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -U__STRICT_ANSI__")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

include_directories(src)

aux_source_directory(src EXE_SRC_LIST)

# librsim sources
aux_source_directory(src/librsim/astro LIBRSIM_SRC_LIST)
aux_source_directory(src/librsim LIBRSIM_SRC_LIST)
aux_source_directory(src/librsim/nequick LIBRSIM_SRC_LIST)
aux_source_directory(src/librsim/astro/sgp_sdp_model LIBRSIM_SRC_LIST)

add_library(${LIB_RSIM_NAME} SHARED ${LIBRSIM_SRC_LIST}
        "src/librsim/nequick/nequick.f"
        "src/librsim/nequick/slqu.f")

add_executable(${PROJECT_NAME} ${EXE_SRC_LIST})

file(GLOB DATA_FILES data *)
file(GLOB TEST_DATA_FILES test/data *)
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${TEST_DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(
        ${LIB_RSIM_NAME}
        gsl gslcblas m
)

target_link_libraries(
        ${PROJECT_NAME}
        ${LIB_RSIM_NAME}
)

set(PROJECT_BINARY_DIR ./build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


set(TEST_OUT_NAME ${PROJECT_NAME}_test)
aux_source_directory(test TEST_SRC_LIST)
add_executable(${TEST_OUT_NAME} ${TEST_SRC_LIST})

target_link_libraries(
        ${TEST_OUT_NAME}
        ${LIB_RSIM_NAME}
        cppunit
)

enable_testing()

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
