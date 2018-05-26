//
// Created by eric on 21/05/18.
//

#include <cstring>
#include <openssl/des.h>
#include "../steg.h"

#include "steg_des.h"

uint8_t* enc_des(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    DES_ncbc_encrypt(text,buffer,size,ks,&iv,DES_ENCRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

uint8_t* dec_des(uint8_t* text, size_t size, void* data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    DES_ncbc_encrypt(text,buffer,size,ks,&iv,DES_DECRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

void steg_des::stegLSB1_des(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    DES_cblock k = "1234567";
    DES_key_schedule ks;
    /* Setear paridad impar*/
    DES_set_odd_parity(&k);
    /* Setear key schedule*/
    DES_set_key_checked(&k, &ks);
    steg::steg_function steg_f = {
            8,
            false,
            enc_des,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, false, steg_f);

}

void steg_des::stegLSB4_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    DES_cblock k = "1234567";
    DES_key_schedule ks;
    /* Setear paridad impar*/
    DES_set_odd_parity(&k);
    /* Setear key schedule*/
    DES_set_key_checked(&k, &ks);
    steg::steg_function steg_f = {
            8,
            false,
            enc_des,
            steg::lsb4_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 4, false, steg_f);
}

void steg_des::stegLSBE_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    steg::steg_function steg_f = {
            8,
            false,
            enc_des,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            nullptr
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, true, steg_f);
}

void steg_des::dec_stegLSB1_des(const char *porter_filename, const char *destiny_filename){

    DES_cblock k = "1234567";
    DES_key_schedule ks;
    /* Setear paridad impar*/
    DES_set_odd_parity(&k);
    /* Setear key schedule*/
    DES_set_key_checked(&k, &ks);

    steg::steg_function steg_f = {
            8,
            false,
            dec_des,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,1, false, steg_f);
}

void steg_des::dec_stegLSB4_des(const char *porter_filename, const char *destiny_filename){
    steg::steg_function steg_f = {
            8,
            false,
            dec_des,
            steg::lsb4_size_compare,
            steg::size_with_padding,
            nullptr
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,4, false, steg_f);
}

void steg_des::dec_stegLSBE_des(const char *porter_filename, const char *destiny_filename){
    steg::steg_function steg_f = {
            8,
            false,
            dec_des,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            nullptr
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,1, true, steg_f);
}

void steg_des::stegLSB8_des(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    steg::steg_function steg_f = {
            8,
            false,
            enc_des,
            steg::lsb8_size_compare,
            steg::size_with_padding,
            nullptr
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,8, false, steg_f);
}

void steg_des::dec_stegLSB8_des(const char *porter_filename, const char *destiny_filename){
    steg::steg_function steg_f = {
            8,
            false,
            dec_des,
            steg::lsb8_size_compare,
            steg::size_with_padding,
            nullptr
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,8, false, steg_f);
}