#include <iostream>
#include <string>
#include <map>
#include "steg/steg.h"
#include "steg/plain/plain_steg.h"
#include "steg/des/steg_des.h"
#include "steg/aes/aes_steg.h"

void print_help(){
    std::cout << "Use: <-extract | -embed -in INPUT_FILE> -p PORTER_FILE -out OUTPUT_FILE -steg LSB1|LSB4|LSB8|LSBE [-pass PASSWORD [-a aes128|aes192|aes256|des] [-m cbc|cfb|ecb|ofb]]"<< std::endl;
    std::cout << std::endl;
    std::cout << "The required arguments are:" << std::endl;
    std::cout << "\t -extract:\tSet the program to get a inner file from the porter file." << std::endl;
    std::cout << "\t -embed:\t Set the program to get a inner file from the porter file. So the -in argument is required." << std::endl;
    std::cout << "\t -p:\t Set the program to get PORTER_FILE as porter file." << std::endl;
    std::cout << "\t -out: Set the program to get OUTPUT_FILE as output file." << std::endl;
    std::cout << "\t -steg: Set the stenography strategy. There are options LSB1, LSB4, LSB8 and LSBE." << std::endl;
    std::cout << std::endl;
    std::cout << "The special arguments are:" << std::endl;
    std::cout << "\t -in: Set the program to get INPUT_FILE as output file. Is required if the -extract argument is passed." << std::endl;
    std::cout << std::endl;
    std::cout << "The optional arguments are:" << std::endl;
    std::cout << "\t -pass: Set PASSWORD as the password to encrypt" << std::endl;
    std::cout << "\t -a: Set the stream cipher function. There are options aes128, aes192, aes256 and des. If -pass passed the default is aes128." << std::endl;
    std::cout << "\t -m: Set the stream cipher strategy. There are options cbc, cfb, ecb and ofb. If -pass passed the default is cbc." << std::endl;
}


void parse_arg(int argc, char** argv, std::map<std::string,std::string>* arg_map) {

    bool error = false;
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
                    error = true;
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
                    error = true;
                }
            }
        } else if (arg == "-in" || arg == "-out" || arg == "-p" || arg == "-m" || arg == "-a" || arg == "-pass" ||
                   arg == "-steg") {
            if ((*arg_map).find(arg) == (*arg_map).end()) {
                (*arg_map)[arg] = argv[i+1];
            }
            else {
                std::cerr << "ERR: Argument " << arg <<" passed more than once" << std::endl;
                error = true;
            }
            i++;
        }
    }

    std::map<std::string,std::string> ready_map = (*arg_map);

    if(ready_map["mode"].empty()){
        error = true;
        std::cerr << "ERR: No 'mode' argument passed" << std::endl;
    }else if(ready_map["mode"] == "embed" and ready_map["-in"].empty()){
        error = true;
        std::cerr << "ERR: embed 'mode' argument passed but no 'in' filename passed" << std::endl;
    }

    if(ready_map["-steg"].empty()){
        error = true;
        std::cerr << "ERR: No 'steg' argument passed" << std::endl;
    } else if (ready_map["-steg"] != "LSB1" and ready_map["-steg"] == "LSB4" and ready_map["-steg"] == "LSBE" and ready_map["-steg"] == "LSB8"){
        error = true;
        std::cerr << "ERR: Incorrect stenography function argument passed" << std::endl;
    }

    if(ready_map["-p"].empty()){
        error = true;
        std::cerr << "ERR: No porter filename argument passed" << std::endl;
    }
    if(ready_map["-out"].empty()){
        error = true;
        std::cerr << "ERR: No out filename argument passed" << std::endl;
    }
    if(ready_map["-pass"].empty()) {
        if (!ready_map["-a"].empty() or !ready_map["-m"].empty()) {
            error = true;
            std::cerr << "ERR: No password argument passed but '-m' or '-a' passed" << std::endl;
        }
    }else {
        if (ready_map["-a"].empty()) {
            ready_map["-a"] = "aes128";
        } else if (ready_map["-a"] != "des" and ready_map["-a"] == "aes128" and ready_map["-a"] == "aes192" and ready_map["-a"] == "aes256"){
            error = true;
            std::cerr << "ERR: Incorrect mode argument passed" << std::endl;
        }
        if (ready_map["-m"].empty()) {
            ready_map["-m"] = "cbc";
        }else if (ready_map["-m"] != "cbc" and ready_map["-m"] != "cfb" and ready_map["-m"] != "ecb" and ready_map["-m"] != "ofb") {
            error = true;
            std::cerr << "ERR: Incorrect cipher function argument passed" << std::endl;
        }
    }

    if(error){
        print_help();
        exit(1);
    }
}

void run(std::map<std::string,std::string> parsed_arg){

    int f_i = -1;
    int lsb_i = -1;
    const char* porter = parsed_arg["-p"].c_str();
    const char* in = parsed_arg["-in"].c_str();
    const char* out = parsed_arg["-out"].c_str();

    if(parsed_arg["-m"] == "cbc"){
        f_i = CBC;
    }else if(parsed_arg["-m"] == "cfb"){
        f_i = CFB;
    }else if(parsed_arg["-m"] == "ecb"){
        f_i = ECB;
    }else if(parsed_arg["-m"] == "ofb"){
        f_i = OFB;
    }

    if(parsed_arg["-steg"] == "LSB1"){
        lsb_i = LSB1;
    }else if(parsed_arg["-steg"] == "LSB4"){
        lsb_i = LSB4;
    }else if(parsed_arg["-steg"] == "LSBE"){
        lsb_i = LSBE;
    }else if(parsed_arg["-steg"] == "LSB8"){
        lsb_i = LSB8;
    }

    if(parsed_arg["mode"] == "embed"){
        if(parsed_arg["-a"].empty()){
            plain_steg::plain_enc(lsb_i,porter,in,out);
        }else if(parsed_arg["-a"] == "des"){
            steg_des::des_enc(lsb_i,f_i,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,in,out);
        }else if(parsed_arg["-a"] == "aes128"){
            steg_aes::aes_enc(lsb_i,f_i,128,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,in,out);
        }else if(parsed_arg["-a"] == "aes192"){
            steg_aes::aes_enc(lsb_i,f_i,192,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,in,out);
        }else if(parsed_arg["-a"] == "aes256"){
            steg_aes::aes_enc(lsb_i,f_i,256,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,in,out);
        }
    }else if (parsed_arg["mode"] == "extract"){
        if(parsed_arg["-a"].empty()){
            plain_steg::plain_dec(lsb_i,porter,out);
        }else if(parsed_arg["-a"] == "des"){
            steg_des::des_dec(lsb_i,f_i,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,out);
        }else if(parsed_arg["-a"] == "aes128"){
            steg_aes::aes_dec(lsb_i,f_i,128,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,out);
        }else if(parsed_arg["-a"] == "aes192"){
            steg_aes::aes_dec(lsb_i,f_i,192,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,out);
        }else if(parsed_arg["-a"] == "aes256"){
            steg_aes::aes_dec(lsb_i,f_i,256,(unsigned char*) (parsed_arg["-pass"].c_str()),porter,out);
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

    run(parsed_arg);

    return 0;
}