//
// Created by eric on 10/05/18.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "steg.h"

#define BUFFERSIZE 128
using namespace std;

    long get_file_size(std::FILE* file){
        long size;
        fseek(file, 0, SEEK_END); // seek to end of file
        size = ftell(file); // get current file pointer
        fseek(file, 0, SEEK_SET);

        return size;
    }

void stegLSB(const char* porter_filename, const char* info_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe) {

    std::FILE* porter_file = std::fopen(porter_filename,"rb");
    std::FILE* info_file = std::fopen(info_filename,"rb");
    std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

    /*CONTROL SIZE*/
    long porter_size = get_file_size(porter_file);
    auto info_size = (uint32_t) get_file_size(info_file);

    if (info_size * 8L > 3L * (porter_size - 53)){
        //TODO ERROR
        cerr << "INFO TOO BIG" << endl;
        exit(1);
    }

    auto porter_buffer = (uint8_t *) malloc(sizeof(uint8_t) * BUFFERSIZE);
    auto info_buffer = (uint8_t*) malloc(sizeof(uint8_t) * BUFFERSIZE);

    bool end = false;
    bool more_info = true;

    size_t porter_read = fread(porter_buffer,1,54,porter_file);
    size_t porter_i = porter_read;
    fwrite(porter_buffer, sizeof(uint8_t),54,destiny_file);
    std::memcpy(info_buffer,&info_size, sizeof(uint32_t));

    size_t info_i = 0;
    size_t info_ii = 0;
    size_t info_read = 4;
    size_t info_written = 0;

    uint8_t signific_bits = std::pow(2,bit_l) - 1;

    size_t written = 0;

    while (!end) {
        if (porter_i == porter_read) {
            porter_i = 0;
            porter_read = fread(porter_buffer, 1, BUFFERSIZE, porter_file);
        }
        if (info_i == info_read) {
            if (ftell(info_file) != info_size) {
                info_i = 0;
                info_ii = 0;
                info_read = fread(info_buffer, 1, BUFFERSIZE, info_file);
            } else {
                more_info = false;
            }
        }

        /*DA MAGIC explained
         * (a: the info byte)                                           a = info_buffer[info_i]
         * (b: bit interested in pos 0)                                 b = a >> 7-info_ii
         * (c: bit interested in pos 0, cleaning more significant bits) c = b & 0x01
         * (d: fill byte with 1)                                        d = c | 254
         * (e: clean least important bit from porter)                   e = porter_buffer[porter_i] | 1
         * (f: combine and resultant byte)                              f = d & e*/
        uint8_t y;
        uint8_t info_aux = info_buffer[info_i], porter_aux = porter_buffer[porter_i];
        if (more_info && ( !is_lsbe || (porter_aux&0xFE) == 0xFE)) {
            info_aux >>= (8 - bit_l - info_ii);
            info_aux &= signific_bits;
            info_aux |= (0xFF - signific_bits);
            porter_aux |= signific_bits;
            info_aux &= porter_aux;
            y = info_aux;
            info_ii += bit_l;
            if (info_ii == 8){
                info_ii = 0;
                info_i++;
                info_written++;
            }
        } else {
            y = porter_aux;
        }

        porter_i++;

        written += fwrite(&y, sizeof(uint8_t),1,destiny_file);

        end = ftell(porter_file) == porter_size;
    }

    fclose(info_file);
    fclose(porter_file);
    fclose(destiny_file);

    if(info_written != info_size + /*FROM size*/4){
        //TODO ERROR
        cerr << "INFO DO NOT ENTER, MAX SIZE = " << info_written << endl;
        exit(1);
    }

}

void dec_stegLSB(const char* porter_filename, const char* destiny_filename, const uint8_t bit_l) {

    std::FILE* porter_file = std::fopen(porter_filename,"rb");
    std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

    long porter_size = get_file_size(porter_file);

    auto tmp_porter_buffer = (uint8_t *) malloc(sizeof(char) * 54);
    auto porter_buffer = (uint8_t *) malloc(sizeof(char) * 8);
    auto size_buffer = (uint8_t *) malloc(sizeof(char) * 4);

    size_t size_i = 0;

    size_t porter_read = fread(tmp_porter_buffer,1,54,porter_file);

    size_t file_size = 0;
    size_t written = 0;

    uint8_t signific_bits = std::pow(2,bit_l) - 1;

    bool end = false;
    while (!end) {
        porter_read = fread(porter_buffer, 1, 8/bit_l, porter_file);

        /*DA MAGIC explained
         * (a: the info byte)                                           a = info_buffer[info_i]
         * (b: bit interested in pos 0)                                 b = a >> 7-info_ii
         * (c: bit interested in pos 0, cleaning more significant bits) c = b & 0x01
         * (d: fill byte with 1)                                        d = c | 254
         * (e: clean least important bit from porter)                   e = porter_buffer[porter_i] | 1
         * (f: combine and resultant byte)                              f = d & e*/
        uint8_t aux = 0;
        for (uint8_t i = 0; i < 8/bit_l; i++) {
            uint8_t aux2 = porter_buffer[i];
            aux <<= bit_l;
            aux2 &= signific_bits;
            aux |= aux2;
        }
        if (size_i < 4) {
            std::memcpy((void*)(size_buffer+size_i),&aux, sizeof(uint8_t));
            size_i++;
            if (size_i == 4){
                file_size = *((uint32_t*) size_buffer);
            }
        } else {
            written += fwrite(&aux, sizeof(uint8_t),1,destiny_file);
            end = written == file_size;
        }
    }

    fclose(porter_file);
    fclose(destiny_file);

}

void steg::stegLSB1(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    stegLSB(porter_filename, info_filename, destiny_filename, 1, false);
}

void steg::stegLSB4(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    stegLSB(porter_filename, info_filename, destiny_filename, 4, false);
}

void steg::stegLSBE(const char* porter_filename, const char* info_filename, const char* destiny_filename) {
    stegLSB(porter_filename, info_filename, destiny_filename, 1, true);
}

void steg::dec_stegLSB1(const char* porter_filename, const char* destiny_filename){
    dec_stegLSB(porter_filename,destiny_filename,1);
}

void steg::dec_stegLSB4(const char* porter_filename, const char* destiny_filename){
    dec_stegLSB(porter_filename,destiny_filename,4);
}

void steg::dec_stegLSBE(const char* porter_filename, const char* destiny_filename){
    //dec_stegLSB(porter_filename,destiny_filename,4);
}
