//
// Created by eric on 21/05/18.
//


#include <cstring>
#include <openssl/aes.h>
#include "../steg.h"

#include "aes_steg.h"

uint8_t* enc_aes_cbc(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_ENCRYPT);
    return buffer;
}

uint8_t* enc_aes_cfb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    AES_cfb1_encrypt(text,buffer,8*size,ks,iv, &a,AES_ENCRYPT);
    return buffer;
}

uint8_t* enc_aes_ecb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    exit(8);
    AES_cfb128_encrypt(text,buffer,size,ks,iv, &a,AES_ENCRYPT);
    return buffer;
}


uint8_t* enc_aes_ofb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    AES_ofb128_encrypt(text,buffer,size,ks,iv,&a);

    return buffer;
}

uint8_t* dec_aes_cbc(uint8_t* text, size_t size, void* data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_DECRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

uint8_t* dec_aes_cfb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    AES_cfb1_encrypt(text,buffer,8*size,ks,iv, &a,AES_DECRYPT);
    return buffer;
}

uint8_t* dec_aes_ecb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    exit(8);
    AES_cfb128_encrypt(text,buffer,size,ks,iv, &a,AES_DECRYPT);
    return buffer;
}


uint8_t* dec_aes_ofb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "012345678901234";
    int a=0;

    AES_ofb128_encrypt(text,buffer,size,ks,iv,&a);

    return buffer;
}

uint8_t* (*f_enc [])(uint8_t *text, size_t buffer_size, void* data) = {enc_aes_cbc,enc_aes_cfb,enc_aes_ecb,enc_aes_ofb};
uint8_t* (*f_dec [])(uint8_t *text, size_t buffer_size, void* data) = {dec_aes_cbc,dec_aes_cfb,dec_aes_ecb,dec_aes_ofb};

void steg_aes::aes_enc(int lsb_mode, int mode, int bits, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_encrypt_key(k, bits, &ks);

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            f_enc[mode],
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &ks
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}

void steg_aes::aes_dec(int lsb_mode, int mode, int bits, const char* porter_filename, const char* destiny_filename){

    auto k = (unsigned char *)"1234567";
    AES_KEY ks;
    /* Setear key schedule*/
    AES_set_decrypt_key(k, bits, &ks);

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            f_dec[mode],
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &ks
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}