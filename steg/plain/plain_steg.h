//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_PLAIN_STEG_H
#define CRIPTO_TP_PLAIN_STEG_H


class plain_steg {

public:

    static void plain_enc(int lsbe_mode, const char* porter_filename, const char* info_filename, const char* destiny_filename);

    static void plain_dec(int lsbe_mode, const char* porter_filename, const char* destiny_filename);
};


#endif //CRIPTO_TP_PLAIN_STEG_H
