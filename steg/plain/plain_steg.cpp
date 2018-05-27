//
// Created by eric on 21/05/18.
//

#include <cstring>
#include "plain_steg.h"
#include "../steg.h"


uint8_t* plain(uint8_t* text, size_t size, void* data){
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    memcpy(buffer,text,size);
    return buffer;
}

void plain_steg::plain_enc(int lsb_mode, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    steg::steg_function steg_f = {
            1, //buffer_size
            true, //is_plain
            plain,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_without_padding,
            nullptr
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}

void plain_steg::plain_dec(int lsb_mode, const char* porter_filename, const char* destiny_filename){

    steg::steg_function steg_f = {
            1, //buffer_size
            true, //is_plain
            plain,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_without_padding,
            nullptr
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}
