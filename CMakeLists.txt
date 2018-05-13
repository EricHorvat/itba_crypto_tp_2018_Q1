cmake_minimum_required(VERSION 3.10)
project(cripto_tp)

set(CMAKE_CXX_STANDARD 11)

add_executable(cripto_tp main.cpp steg/steg.cpp steg/steg.h)