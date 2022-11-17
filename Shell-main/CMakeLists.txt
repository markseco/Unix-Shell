cmake_minimum_required(VERSION 3.23)
project(Shell C)

set(CMAKE_C_STANDARD 11)

add_executable(Shell
        main.c head_list.c head_list.h AuxiliarFunctions.c AuxiliarFunctions.h libraries-headers.h commands.c commands.h list_blocks.c      
        list_blocks.h)
