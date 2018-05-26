//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_AES_STEG_H
#define CRIPTO_TP_AES_STEG_H


class steg_aes {

public:
    static void stegLSB1_aes(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB1_aes(const char* porter_filename, const char* destiny_filename);

    static void stegLSB4_aes(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB4_aes(const char* porter_filename, const char* destiny_filename);

    static void stegLSBE_aes(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSBE_aes(const char* porter_filename, const char* destiny_filename);

    static void stegLSB8_aes(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB8_aes(const char* porter_filename, const char* destiny_filename);

};


#endif //CRIPTO_TP_AES_STEG_H
