cmake_minimum_required(VERSION 2.8.4)
project(control)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/control/inc")
include_directories("/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/freertos/inc")
include_directories("/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/base/inc")
include_directories("/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/board/inc")
include_directories("/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/chip/inc")


FILE(GLOB_RECURSE SRC_LIST "/home/ivan-ubuntu/Documents/embebidos/tp_final/codigo/edu-CIAA/firmware/control/src/*.*")

add_executable(clion ${SRC_LIST})

add_custom_target(control COMMAND make -C ${control_SOURCE_DIR}
                         CLION_EXE_DIR=${PROJECT_BINARY_DIR})
