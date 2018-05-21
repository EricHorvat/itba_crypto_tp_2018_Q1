//
// Created by eric on 21/05/18.
//

#include <cstring>
#include "plain_steg.h"
#include "../steg.h"


uint8_t* plain(uint8_t* text, size_t size){
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    memcpy(buffer,text,size);
    for (int i = 0; i < size; ++i) {
        buffer[i] += 0;
    }
    return buffer;
}

void plain_steg::stegLSB1_plain(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, false, {1,true,plain,steg::lsb1_size_compare,steg::size_without_padding});
}

void plain_steg::stegLSB4_plain(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 4, false, {1,true,plain,steg::lsb4_size_compare,steg::size_without_padding});
}

void plain_steg::stegLSBE_plain(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, true, {1,true,plain,steg::lsb1_size_compare,steg::size_without_padding});
}

void plain_steg::dec_stegLSB1_plain(const char *porter_filename, const char *destiny_filename){
    steg::dec_stegLSB(porter_filename,destiny_filename,1, false, {1,true,plain,steg::lsb1_size_compare,steg::size_without_padding});
}

void plain_steg::dec_stegLSB4_plain(const char *porter_filename, const char *destiny_filename){
    steg::dec_stegLSB(porter_filename,destiny_filename,4, false, {1,true,plain,steg::lsb4_size_compare,steg::size_without_padding});
}

void plain_steg::dec_stegLSBE_plain(const char *porter_filename, const char *destiny_filename){
    steg::dec_stegLSB(porter_filename,destiny_filename,1, true, {1,true,plain,steg::lsb1_size_compare,steg::size_without_padding});
}

void plain_steg::stegLSB8_plain(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 8, false, {1,true,plain,steg::lsb8_size_compare,steg::size_without_padding});
}

void plain_steg::dec_stegLSB8_plain(const char *porter_filename, const char *destiny_filename){
    steg::dec_stegLSB(porter_filename,destiny_filename, 8, false, {1,true,plain,steg::lsb8_size_compare,steg::size_without_padding});
}
