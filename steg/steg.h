//
// Created by eric on 10/05/18.
//

#ifndef CRIPTO_TP_STEG_H
#define CRIPTO_TP_STEG_H


#include <string>

class steg {

public:
    static void stegLSB1(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB1(const char* porter_filename, const char* destiny_filename);

    static void stegLSB4(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB4(const char* porter_filename, const char* destiny_filename);

    static void stegLSBE(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSBE(const char* porter_filename, const char* destiny_filename);

};


#endif //CRIPTO_TP_STEG_H
