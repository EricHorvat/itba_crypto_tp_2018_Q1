#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>

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
    for(std::map<std::string,std::string>::iterator iter = parsed_arg.begin(); iter != parsed_arg.end(); ++iter)
    {
        std::cout << iter->first << " = " << iter->second << std::endl;
    }

    return 0;
}