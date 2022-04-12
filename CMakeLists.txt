
cmake_minimum_required(VERSION 3.21)

list(APPEND DASHBOARD_COMMON_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/constants.h
        ${CMAKE_CURRENT_LIST_DIR}/src/buffer.c
        ${CMAKE_CURRENT_LIST_DIR}/src/buffer.h
        ${CMAKE_CURRENT_LIST_DIR}/src/protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/src/protocol.h
        ${CMAKE_CURRENT_LIST_DIR}/src/log.c
        ${CMAKE_CURRENT_LIST_DIR}/src/log.h
        ${CMAKE_CURRENT_LIST_DIR}/src/profile.c
        ${CMAKE_CURRENT_LIST_DIR}/src/profile.h
        )

