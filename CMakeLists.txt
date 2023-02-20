cmake_minimum_required(VERSION 3.23)
project(TexIL C)

set(CMAKE_C_STANDARD 23)

add_library(TexIL TexIL.c VM/TexVM.h)
