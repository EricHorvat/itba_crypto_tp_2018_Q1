//
// Created by eric on 21/05/18.
//


#include <cstring>
#include <openssl/aes.h>
#include <openssl/ssl.h>
#include "../steg.h"

#include "aes_steg.h"

typedef struct {
    int bits;
    int mode;
    unsigned char * pass;
} AES_DATA;

const EVP_CIPHER* f_array[][4] = {{EVP_aes_128_cbc(),EVP_aes_128_cfb8(),EVP_aes_128_ecb(),EVP_aes_128_ofb()},
                                         {EVP_aes_192_cbc(),EVP_aes_192_cfb8(),EVP_aes_192_ecb(),EVP_aes_192_ofb()},
                                                 {EVP_aes_256_cbc(),EVP_aes_256_cfb8(),EVP_aes_256_ecb(),EVP_aes_256_ofb()}};

uint8_t* enc_aes_f(uint8_t* text, size_t size, void *data){
    AES_DATA *aes_data = ((AES_DATA*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[16];//[] = "0123456789012345\0";
    unsigned char *k;
    int outl, templ;

    int bits, passl = strlen((const char*)aes_data->pass);

    if (aes_data->bits == 128){
        bits = AES128;
        k = (unsigned char *) malloc(16);
    }else if (aes_data->bits == 192){
        bits = AES192;
        k = (unsigned char *) malloc(24);
    }else if (aes_data->bits == 256){
        bits = AES256;
        k = (unsigned char *) malloc(32);
    }

    EVP_CIPHER_CTX* ctx;
    ctx = EVP_CIPHER_CTX_new();

    EVP_BytesToKey(f_array[bits][aes_data->mode], EVP_sha256(), nullptr, aes_data->pass,passl,1,k,iv);

    EVP_EncryptInit_ex(ctx, f_array[bits][aes_data->mode], NULL, k, iv);
    EVP_EncryptUpdate(ctx, buffer, &outl, text, size);
    EVP_EncryptFinal(ctx, buffer + outl, &templ);
    EVP_CIPHER_CTX_free(ctx);

    return buffer;
}

uint8_t* dec_aes_f(uint8_t* text, size_t size, void *data){
    AES_DATA *aes_data = ((AES_DATA*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[16];// = "0123456789012345\0";
    unsigned char *k;
    int outl, templ;

    int bits, passl = strlen((const char*)aes_data->pass);

    if (aes_data->bits == 128){
        bits = AES128;
        k = (unsigned char *) malloc(16);
//        k = (unsigned char *) "012345678901234\0";
    }else if (aes_data->bits == 192){
        bits = AES192;
  //      k = (unsigned char *) "012345678901234567890123\0";
        k = (unsigned char *) malloc(24);
    }else if (aes_data->bits == 256){
        bits = AES256;
//        k = (unsigned char *) "01234567890123456789012345678901\0";
        k = (unsigned char *) malloc(32);
    }

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();


    EVP_BytesToKey(f_array[bits][aes_data->mode], EVP_sha256(), nullptr, aes_data->pass,passl,1,k,iv);


    EVP_DecryptInit_ex(ctx, f_array[bits][aes_data->mode], NULL, k, iv);
    EVP_DecryptUpdate(ctx, buffer, &outl, text, size);
    EVP_DecryptFinal(ctx, buffer + outl, &templ);
    EVP_CIPHER_CTX_free(ctx);

    return buffer;
}


void steg_aes::aes_enc(int lsb_mode, int mode, int bits, unsigned char* pass, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    AES_DATA aes_data = {bits,mode,pass};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            enc_aes_f,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &aes_data
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}

void steg_aes::aes_dec(int lsb_mode, int mode, int bits, unsigned char* pass,const char* porter_filename, const char* destiny_filename){

    AES_DATA aes_data = {bits, mode, pass};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            dec_aes_f,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &aes_data
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}