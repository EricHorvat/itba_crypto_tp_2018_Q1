//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_AES_STEG_H
#define CRIPTO_TP_AES_STEG_H

#define AES128 0
#define AES192 1
#define AES256 2


class steg_aes {

public:

    static void aes_enc(int lsbe_mode, int mode, int bits, unsigned char* pass, const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void aes_dec(int lsbe_mode, int mode, int bits, unsigned char* pass, const char* porter_filename, const char* destiny_filename);

};


#endif //CRIPTO_TP_AES_STEG_H
