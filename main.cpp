#include <iostream>
#include <string>
#include <map>
#include "steg/steg.h"
#include "steg/plain/plain_steg.h"
#include "steg/des/steg_des.h"
#include "steg/aes/aes_steg.h"

void print_help(){

}


int parse_arg(int argc, char** argv, std::map<std::string,std::string>* arg_map) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-embed") {
            if ((*arg_map).find("mode") == (*arg_map).end()) {
                (*arg_map)["mode"] = "embed";
            }
            else {
                if((*arg_map).find("mode")->second == "embed"){
                    std::clog << "WARN: -embed argument passed more than once" << std::endl;
                }else {
                    std::cerr << "ERR: Both -embed and -extract argument passed" << std::endl;
                    print_help();
                    std::exit(1);
                }
            }
        } else if (arg == "-extract") {
            if ((*arg_map).find("mode") == (*arg_map).end()) {
                (*arg_map)["mode"] = "extract";
            }
            else {
                if((*arg_map).find("mode")->second == "extract"){
                    std::clog << "WARN: -embed argument passed more than once" << std::endl;
                }else {
                    std::cerr << "ERR: Both -embed and -extract argument passed" << std::endl;
                    print_help();
                    std::exit(1);
                }
            }
        } else if (arg == "-in" || arg == "-out" || arg == "-p" || arg == "-m" || arg == "-a" || arg == "-pass" ||
                   arg == "-steg") {
            if ((*arg_map).find(arg) == (*arg_map).end()) {
                (*arg_map)[arg] = argv[i+1];
            }
            else {
                std::cerr << "ERR: Argument " << arg <<" passed more than once" << std::endl;
                print_help();
                std::exit(1);
            }
            i++;
        }
    }
}

int main( int argc, char **argv) {

    std::map<std::string,std::string> parsed_arg;
    parse_arg(argc,argv,&parsed_arg);
    for(const auto& elem : parsed_arg)
    {
        std::cout << elem.first << " = " << elem.second << std::endl;
    }

    plain_steg::stegLSB1_plain("../in/big.bmp","../in/mid.bmp","../out/bigg.bmp");
    plain_steg::dec_stegLSB1_plain("../out/bigg.bmp","../out/midd");
    steg_des::stegLSB1_des("../in/big.bmp","../in/mid.bmp","../out/biggDES.bmp");
    steg_des::dec_stegLSB1_des("../out/biggDES.bmp","../out/middDES");
    steg_aes::stegLSB1_aes("../in/big.bmp","../in/mid.bmp","../out/biggAES.bmp");
    steg_aes::dec_stegLSB1_aes("../out/biggAES.bmp","../out/middAES");

    return 0;
}