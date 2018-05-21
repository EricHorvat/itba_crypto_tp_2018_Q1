//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_PLAIN_STEG_H
#define CRIPTO_TP_PLAIN_STEG_H


class plain_steg {

public:
    static void stegLSB1_plain(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB1_plain(const char* porter_filename, const char* destiny_filename);

    static void stegLSB4_plain(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB4_plain(const char* porter_filename, const char* destiny_filename);

    static void stegLSBE_plain(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSBE_plain(const char* porter_filename, const char* destiny_filename);

    static void stegLSB8_plain(const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void dec_stegLSB8_plain(const char* porter_filename, const char* destiny_filename);
};


#endif //CRIPTO_TP_PLAIN_STEG_H
