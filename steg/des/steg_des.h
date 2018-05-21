//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_DES_STEG_H
#define CRIPTO_TP_DES_STEG_H


class steg_des {

    public:
        static void stegLSB1_des(const char* porter_filename, const char* info_filename, const char* destiny_filename);

        static void dec_stegLSB1_des(const char* porter_filename, const char* destiny_filename);

        static void stegLSB4_des(const char* porter_filename, const char* info_filename, const char* destiny_filename);

        static void dec_stegLSB4_des(const char* porter_filename, const char* destiny_filename);

        static void stegLSBE_des(const char* porter_filename, const char* info_filename, const char* destiny_filename);

        static void dec_stegLSBE_des(const char* porter_filename, const char* destiny_filename);

        static void stegLSB8_des(const char* porter_filename, const char* info_filename, const char* destiny_filename);

        static void dec_stegLSB8_des(const char* porter_filename, const char* destiny_filename);

};


#endif //CRIPTO_TP_DES_STEG_H
