//
// Created by eric on 21/05/18.
//

#include "../steg.h"

#include "steg_des.h"

void steg_des::stegLSB1_des(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    //steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, false, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::stegLSB4_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    //stegLSB(porter_filename, info_filename, destiny_filename, 4, false, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::stegLSBE_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    //stegLSB(porter_filename, info_filename, destiny_filename, 1, true, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::dec_stegLSB1_des(const char *porter_filename, const char *destiny_filename){
    //dec_stegLSB(porter_filename,destiny_filename,1, false, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::dec_stegLSB4_des(const char *porter_filename, const char *destiny_filename){
    //dec_stegLSB(porter_filename,destiny_filename,4, false, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::dec_stegLSBE_des(const char *porter_filename, const char *destiny_filename){
    //dec_stegLSB(porter_filename,destiny_filename,1, true, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::stegLSB8_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    //stegLSB(porter_filename, info_filename, destiny_filename, 8, false, {1,true,plain,size_compare,size_without_padding});
}

void steg_des::dec_stegLSB8_des(const char *porter_filename, const char *destiny_filename){
    //dec_stegLSB(porter_filename,destiny_filename, 8, false, {1,true,plain,size_compare,size_without_padding});
}