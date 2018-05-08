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
            //TODO CONTROL
            (*arg_map)["mode"] = "embed";
        } else if (arg == "-extract") {
            //TODO CONTROL
            (*arg_map)["mode"] = "extract";
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
    std::string s = parsed_arg.find("mode")->second;


    return 0;
}