//
// Created by eric on 10/05/18.
//

#ifndef CRIPTO_TP_STEG_H
#define CRIPTO_TP_STEG_H

#define CBC 0
#define CFB 1
#define ECB 2
#define OFB 3

#define LSB1 0
#define LSB4 1
#define LSBE 2
#define LSB8 3

#include <string>

class steg {

public:

    typedef struct {
        size_t buffer_size;

        bool is_plain;

        uint8_t *(*f)(uint8_t *text, size_t buffer_size, void* data);

        bool (*compare_sizes)(long porter_size, size_t info_size);

        size_t (*get_info_c_size)(size_t buffer_size, size_t extension_length, size_t info_size);

        void* data;
    } steg_function;

    static void
    stegLSB(const char *porter_filename, const char *info_filename, const char *destiny_filename, const uint8_t bit_l,
            const bool is_lsbe, steg::steg_function steg_f);

    static void
    dec_stegLSB(const char *porter_filename, const char *destiny_filename, const uint8_t bit_l, const bool is_lsbe,
                steg::steg_function steg_f);

    static bool lsb1_size_compare(long porter_size, size_t info_size);

    static bool lsb4_size_compare(long porter_size, size_t info_size);

    static bool lsb8_size_compare(long porter_size, size_t info_size);

    static size_t size_with_padding(size_t info_size, size_t extension_length, size_t buffer_size);

    static size_t size_without_padding(size_t info_size, size_t extension_length, size_t buffer_size);

    static bool (*compare_sizes_lsbe [])(long porter_size, size_t info_size);

    static bool is_lsbe[];
    static uint8_t lsb_bits[];

};
#endif //CRIPTO_TP_STEG_H
