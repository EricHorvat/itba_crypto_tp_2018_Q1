//
// Created by eric on 21/05/18.
//


#include <cstring>
#include <openssl/aes.h>
#include <openssl/ssl.h>
#include "../steg.h"

#include "aes_steg.h"

uint8_t* enc_aes_cbc(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_ENCRYPT);

    return buffer;

}

uint8_t* enc_aes_cfb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    int a=0;

    AES_cfb8_encrypt(text,buffer,size,ks,iv, &a,AES_ENCRYPT);
    return buffer;
}

uint8_t* enc_aes_ecb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    int a=0;

    exit(8);
    AES_cfb128_encrypt(text,buffer,size,ks,iv, &a,AES_ENCRYPT);
    return buffer;
}


uint8_t* enc_aes_ofb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    int a=0, outl, templ;

    unsigned char k[] = "0123456789987654\0";
    EVP_CIPHER_CTX ctx;
    /* Inicializar contexto */
    EVP_CIPHER_CTX_init(&ctx);
    /* Establecer parámetros de encripción en el contexto*/
    EVP_EncryptInit_ex(&ctx, EVP_aes_128_ofb(), NULL, k, iv);
    EVP_EncryptUpdate(&ctx, buffer, &outl, text, size);
    /* Encripta 112 */
    printf("Encripta primero %d bytes\n",outl);
    /* Encripta la parte final, lo que queda del bloque + padding, en este caso 4 bytes + 12 bytes de padding */
    EVP_EncryptFinal(&ctx, buffer + outl, &templ);
    /* Borrar estructura de contexto */
    EVP_CIPHER_CTX_cleanup(&ctx);

    return buffer;
}


typedef struct {
    int bits;
    int mode;
} AES_DATA;

typedef struct {
    const EVP_CIPHER* (*f)();
} AES_F;

const EVP_CIPHER* f_array[][4] = {{EVP_aes_128_cbc(),EVP_aes_128_cfb8(),EVP_aes_128_ecb(),EVP_aes_128_ofb()},
                                         {EVP_aes_192_cbc(),EVP_aes_192_cfb8(),EVP_aes_192_ecb(),EVP_aes_192_ofb()},
                                                 {EVP_aes_256_cbc(),EVP_aes_256_cfb8(),EVP_aes_256_ecb(),EVP_aes_256_ofb()}};

uint8_t* enc_aes_t(uint8_t* text, size_t size, void *data){
    AES_DATA *aes_data = ((AES_DATA*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    unsigned char *k;
    int outl, templ;

    int bits;

    if (aes_data->bits == 128){
        bits = AES128;
        k = (unsigned char *) "012345678901234\0";
    }else if (aes_data->bits == 192){
        bits = AES192;
        k = (unsigned char *) "012345678901234567890123\0";
    }else if (aes_data->bits == 256){
        bits = AES256;
        k = (unsigned char *) "01234567890123456789012345678901\0";
    }

    EVP_CIPHER_CTX ctx;
    /* Inicializar contexto */
    EVP_CIPHER_CTX_init(&ctx);
    /* Establecer parámetros de encripción en el contexto*/
    EVP_EncryptInit_ex(&ctx, f_array[bits][aes_data->mode], NULL, k, iv);
    EVP_EncryptUpdate(&ctx, buffer, &outl, text, size);
    /* Encripta 112 */
    printf("Encripta primero %d bytes\n",outl);
    /* Encripta la parte final, lo que queda del bloque + padding, en este caso 4 bytes + 12 bytes de padding */
    EVP_EncryptFinal(&ctx, buffer + outl, &templ);
    /* Borrar estructura de contexto */
    EVP_CIPHER_CTX_cleanup(&ctx);

    return buffer;
}
uint8_t* dec_aes_t(uint8_t* text, size_t size, void *data){
    AES_DATA *aes_data = ((AES_DATA*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    unsigned char *k;
    int outl, templ;

    int bits;

    if (aes_data->bits == 128){
        bits = AES128;
        k = (unsigned char *) "012345678901234\0";
    }else if (aes_data->bits == 192){
        bits = AES192;
        k = (unsigned char *) "012345678901234567890123\0";
    }else if (aes_data->bits == 256){
        bits = AES256;
        k = (unsigned char *) "01234567890123456789012345678901\0";
    }

    EVP_CIPHER_CTX ctx;
    /* Inicializar contexto */
    EVP_CIPHER_CTX_init(&ctx);
    /* Establecer parámetros de encripción en el contexto*/
    EVP_DecryptInit_ex(&ctx, f_array[bits][aes_data->mode], NULL, k, iv);
    EVP_DecryptUpdate(&ctx, buffer, &outl, text, size);
    /* Encripta 112 */
    printf("Encripta primero %d bytes\n",outl);
    /* Encripta la parte final, lo que queda del bloque + padding, en este caso 4 bytes + 12 bytes de padding */
    EVP_DecryptFinal(&ctx, buffer + outl, &templ);
    /* Borrar estructura de contexto */
    EVP_CIPHER_CTX_cleanup(&ctx);

    return buffer;
}

uint8_t* dec_aes_cbc(uint8_t* text, size_t size, void* data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";

    AES_cbc_encrypt(text,buffer,size,ks,iv,AES_DECRYPT);
    //for (int i = 0; i < size; ++i) {
    //    buffer[i] += 0;
    //}
    return buffer;
}

uint8_t* dec_aes_cfb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    int a=0;

    AES_cfb8_encrypt(text,buffer,size,ks,iv, &a,AES_DECRYPT);
    return buffer;
}

uint8_t* dec_aes_ecb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    int a=0;

    exit(8);
    AES_cfb128_encrypt(text,buffer,size,ks,iv, &a,AES_DECRYPT);
    return buffer;
}


uint8_t* dec_aes_ofb(uint8_t* text, size_t size, void *data){
    AES_KEY *ks = ((AES_KEY*)data);
    auto buffer = (uint8_t *) malloc(sizeof(char) * size);
    unsigned char iv[] = "0123456789012345\0";
    unsigned char k[]  = "0123456789987654\0";
    int a=0,outl, templ;

    EVP_CIPHER_CTX ctx;
    /* Inicializar contexto */
    EVP_CIPHER_CTX_init(&ctx);
    /* Establecer parámetros de encripción en el contexto*/
    EVP_DecryptInit_ex(&ctx, EVP_aes_128_ofb(), NULL, k, iv);
    EVP_DecryptUpdate(&ctx, buffer, &outl, text, size);
    /* Encripta 112 */
    printf("Encripta primero %d bytes\n",outl);
    /* Encripta la parte final, lo que queda del bloque + padding, en este caso 4 bytes + 12 bytes de padding */
    EVP_DecryptFinal(&ctx, buffer + outl, &templ);
    /* Borrar estructura de contexto */
    EVP_CIPHER_CTX_cleanup(&ctx);


    return buffer;
}

uint8_t* (*f_enc [])(uint8_t *text, size_t buffer_size, void* data) = {enc_aes_cbc,enc_aes_cfb,enc_aes_ecb,enc_aes_ofb};
uint8_t* (*f_dec [])(uint8_t *text, size_t buffer_size, void* data) = {dec_aes_cbc,dec_aes_cfb,dec_aes_ecb,dec_aes_ofb};

void steg_aes::aes_enc(int lsb_mode, int mode, int bits, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    AES_DATA aes_data = {bits,mode};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            enc_aes_t,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &aes_data
    };

    steg::stegLSB(porter_filename,info_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}
void aes_enc_old(int lsb_mode, int mode, int bits, const char* porter_filename, const char* info_filename, const char* destiny_filename){

    unsigned char k[] = "0123456789987654\0";
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

    AES_DATA aes_data = {bits, mode};

    steg::steg_function steg_f = {
            16, //buffer_size => 128bits = 16 bytes
            false, //is_plain
            dec_aes_t,
            steg::compare_sizes_lsbe[lsb_mode],
            steg::size_with_padding,
            &aes_data
    };

    steg::dec_stegLSB(porter_filename,destiny_filename, steg::lsb_bits[lsb_mode], steg::is_lsbe[lsb_mode], steg_f);
}
void old_aes_dec(int lsb_mode, int mode, int bits, const char* porter_filename, const char* destiny_filename){

    unsigned char k[] = "0123456789987654\0";
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