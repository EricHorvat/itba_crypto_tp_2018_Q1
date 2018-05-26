//
// Created by eric on 21/05/18.
//


#include <cstring>
#include <openssl/aes.h>
#include "../steg.h"

#include "aes_steg.h"

uint8_t* enc_aes(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_ENCRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

uint8_t* dec_aes(uint8_t* text, size_t size, void* data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_DECRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

void steg_aes::stegLSB1_aes(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_encrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            enc_aes,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, false, steg_f);

}

void steg_aes::stegLSB4_aes(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_encrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            enc_aes,
            steg::lsb4_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 4, false, steg_f);
}

void steg_aes::stegLSBE_aes(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_encrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            enc_aes,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::stegLSB(porter_filename, info_filename, destiny_filename, 1, true, steg_f);
}

void steg_aes::dec_stegLSB1_aes(const char *porter_filename, const char *destiny_filename){

    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, 128, &ks);

    steg::steg_function steg_f = {
            16,
            false,
            dec_aes,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,1, false, steg_f);
}

void steg_aes::dec_stegLSB4_aes(const char *porter_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            dec_aes,
            steg::lsb4_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,4, false, steg_f);
}

void steg_aes::dec_stegLSBE_aes(const char *porter_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            dec_aes,
            steg::lsb1_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,1, true, steg_f);
}

void steg_aes::stegLSB8_aes(const char *porter_filename, const char *info_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            16,
            false,
            enc_aes,
            steg::lsb8_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,8, false, steg_f);
}

void steg_aes::dec_stegLSB8_aes(const char *porter_filename, const char *destiny_filename){
    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, 128, &ks);
    steg::steg_function steg_f = {
            8,
            false,
            dec_aes,
            steg::lsb8_size_compare,
            steg::size_with_padding,
            &ks
    };
    steg::dec_stegLSB(porter_filename,destiny_filename,8, false, steg_f);
}