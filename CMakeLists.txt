cmake_minimum_required(VERSION 3.9)
project(cripto_tp)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenSSL REQUIRED)

add_executable(cripto_tp main.cpp steg/steg.cpp steg/steg.h steg/aes/aes_steg.cpp steg/aes/aes_steg.h steg/des/steg_des.cpp steg/des/steg_des.h steg/plain/plain_steg.cpp steg/plain/plain_steg.h)
target_link_libraries(cripto_tp OpenSSL::SSL)
