cmake_minimum_required(VERSION 2.8.4)
project(control)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/control/inc")
include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/freertos/inc")
include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/base/inc")
include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/board/inc")
include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/chip/inc")
include_directories("/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/sapi/inc")


FILE(GLOB_RECURSE SRC_LIST "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/control/src/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/control/inc/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/freertos/src/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/base/src/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/board/src/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/chip/src/*.*"
        "/home/ivan/Documentos/embebidos_tp_final/codigo/edu-CIAA/firmware/modules/lpc4337_m4/sapi/src/*.*")

add_executable(clion ${SRC_LIST})

add_custom_target(control COMMAND make -C ${control_SOURCE_DIR}
                         CLION_EXE_DIR=${PROJECT_BINARY_DIR})
