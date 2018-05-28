//
// Created by eric on 21/05/18.
//

#include <cstring>
#include <openssl/des.h>
#include <openssl/evp.h>
#include "../steg.h"

#include "steg_des.h"

uint8_t* enc_des_cbc(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    DES_ncbc_encrypt(text,buffer,size,ks,&iv,DES_ENCRYPT);
    return buffer;
}
uint8_t* enc_des_cfb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //https://blog.fpmurphy.com/2010/04/openssl-des-api.html
    DES_cfb_encrypt(text,buffer,1,size,ks,&iv,DES_ENCRYPT);
    return buffer;
}
uint8_t* enc_des_ecb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //DES_ofb_encrypt(text,buffer,size,ks,&iv,DES_ENCRYPT);
    exit(8);
    return buffer;
}
uint8_t* enc_des_ofb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //https://blog.fpmurphy.com/2010/04/openssl-des-api.html 
    DES_ofb_encrypt(text,buffer,8,size,ks,&iv);
    return buffer;
}
uint8_t* dec_des_cbc(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    DES_ncbc_encrypt(text,buffer,size,ks,&iv,DES_DECRYPT);
    return buffer;
}
uint8_t* dec_des_cfb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //https://blog.fpmurphy.com/2010/04/openssl-des-api.html
    DES_cfb_encrypt(text,buffer,1,size,ks,&iv,DES_DECRYPT);
    return buffer;
}
uint8_t* dec_des_ecb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //DES_ofb_encrypt(text,buffer,size,ks,&iv,DES_DECRYPT);
    exit(8);
    return buffer;
}
uint8_t* dec_des_ofb(uint8_t* text, size_t size, void *data){
    DES_key_schedule *ks = ((DES_key_schedule*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    DES_cblock iv ="8765432";

    //https://blog.fpmurphy.com/2010/04/openssl-des-api.html
    DES_ofb_encrypt(text,buffer,8,size,ks,&iv);
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

uint8_t* (*des_f_enc [])(uint8_t *text, size_t buffer_size, void* data) = {enc_des_cbc,enc_des_cfb,enc_des_ecb,enc_des_ofb};
uint8_t* (*des_f_dec [])(uint8_t *text, size_t buffer_size, void* data) = {dec_des_cbc,dec_des_cfb,dec_des_ecb,dec_des_ofb};

void old_des_enc(int lsb_mode, int mode, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    DES_cblock k = "1234567";
    DES_key_schedule ks;
    /* Setear paridad impar*/
    DES_set_odd_parity(&k);
    /* Setear key schedule*/
    DES_set_key_checked(&k, &ks);
    steg::steg_function steg_f = {
            8, //buffer_size => 64bits = 8 bytes
            false, //is_plain
            des_f_enc[mode],
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &ks
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}

void old_des_dec(int lsb_mode, int mode, const char* porter_filename, const char* destiny_filename){

    DES_cblock k = "1234567";
    DES_key_schedule ks;
    /* Setear paridad impar*/
    DES_set_odd_parity(&k);
    /* Setear key schedule*/
    DES_set_key_checked(&k, &ks);
    steg::steg_function steg_f = {
            8,
            steg::is_lsbe[lsb_mode],
            des_f_dec[mode],
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &ks
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], false, steg_f);
}


typedef struct {
    int mode;
} DES_DATA;

const EVP_CIPHER* des_f_array[] = {EVP_des_cbc(),EVP_des_cfb8(),EVP_des_ecb(),EVP_des_ofb()};

uint8_t* enc_des_f(uint8_t* text, size_t size, void *data){
    auto des_data = (DES_DATA*)data;
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "01234567\0";
    unsigned char *k;
    int outl, templ;

    k = (unsigned char *) "01234567\0";

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, des_f_array[des_data->mode], NULL, k, iv);
    EVP_EncryptUpdate(&ctx, buffer, &outl, text, size);
    EVP_EncryptFinal(&ctx, buffer + outl, &templ);
    EVP_CIPHER_CTX_cleanup(&ctx);

    return buffer;
}

uint8_t* dec_des_f(uint8_t* text, size_t size, void *data){
    auto des_data = (DES_DATA*)data;
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    unsigned char *k;
    int outl, templ;

    k = (unsigned char *) "01234567\0";

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_DecryptInit_ex(&ctx, des_f_array[des_data->mode], NULL, k, iv);
    EVP_DecryptUpdate(&ctx, buffer, &outl, text, size);
    EVP_DecryptFinal(&ctx, buffer + outl, &templ);
    EVP_CIPHER_CTX_cleanup(&ctx);

    return buffer;
}


void steg_des::des_enc(int lsb_mode, int mode, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    DES_DATA des_data = {mode};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            enc_des_f,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &des_data
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}

void steg_des::des_dec(int lsb_mode, int mode, const char* porter_filename, const char* destiny_filename){

    DES_DATA des_data = {mode};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            dec_des_f,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &des_data
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}