//
// Created by eric on 21/05/18.
//

#ifndef CRIPTO_TP_DES_STEG_H
#define CRIPTO_TP_DES_STEG_H


class steg_des {

    public:

        static void des_enc(int lsbe_mode, int mode, const char* porter_filename, const char* info_filename, const char* destiny_filename);

        static void des_dec(int lsbe_mode, int mode,const char* porter_filename, const char* destiny_filename);

};


#endif //CRIPTO_TP_DES_STEG_H
