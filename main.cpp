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
                    std::cerr << "-embed argument passed more than once";
                }else {
                    std::cerr << "Both -embed and -extract argument passed";
                    print_help();
                }
            }
        } else if (arg == "-extract") {
            if ((*arg_map).find("mode") == (*arg_map).end()) {
                (*arg_map)["mode"] = "extract";
            }
            else {
                if((*arg_map).find("mode")->second == "extract"){
                    std::cerr << "-embed argument passed more than once";
                }else {
                    std::cerr << "Both -embed and -extract argument passed";
                    print_help();
                }
            }
        } else if (arg == "-test") {
            //TODO CONTROL
            (*arg_map)["test"] = "true";
        } else if (arg == "-in" || arg == "-out" || arg == "-p" || arg == "-m" || arg == "-a" || arg == "-pass" ||
                   arg == "-steg") {
            //TODO CONTROL
            (*arg_map)[arg] = argv[i+1];
            i++;
        }
    }

}

int main( int argc, char **argv) {

    std::map<std::string,std::string> parsed_arg;
    parse_arg(argc,argv,&parsed_arg);
    if (parsed_arg.find("mode") != parsed_arg.end())
        std::string s = parsed_arg.find("mode")->second;
    else
        std::cout << "q";
    if (parsed_arg.find("test") != parsed_arg.end())
        std::string sq = parsed_arg.find("test")->second;
    else
        std::cout <<"w";


    return 0;
}