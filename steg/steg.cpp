//
// Created by eric on 10/05/18.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "steg.h"

#include <openssl/aes.h>

using namespace std;

    bool (*steg::compare_sizes_lsbe [])(long,size_t) = {steg::lsb1_size_compare,steg::lsb4_size_compare,steg::lsb1_size_compare,steg::lsb8_size_compare};

    bool steg::is_lsbe[] = {false, false, true, false};
    uint8_t steg::lsb_bits[] = {1, 4, 1, 8};


    long get_file_size(std::FILE* file){
        long size;
        fseek(file, 0, SEEK_END); // seek to end of file
        size = ftell(file); // get current file pointer
        fseek(file, 0, SEEK_SET);

        return size;
    }

    uint8_t add_bit(uint8_t info_aux, uint8_t porter_aux, uint8_t bit_l, size_t info_bit){
        uint8_t signific_bits = std::pow(2,bit_l) - 1;
        /*DA MAGIC explained                                              LSB1/LSBE                    LSB4
         * (a: the info byte)                                           a = info_aux
         * (b: bit interested in pos 0 / bits i pos 0 to 3)             b = a >> 8-1-(0..7)         b = a >> 8-4-(0|4)
         */
        info_aux >>= (8 - bit_l - info_bit);
        /*
         * (c: bit interested in pos 0, cleaning more significant bits) c = b & 0x01                c = b & 0x0F(00001111)
         * */
        info_aux &= signific_bits;

        /*
         * (d: fill byte with 1)                                        d = c | 0xFE(11111110)      d = c | 0xF0(11110000)
         */
        info_aux |= (0xFF - signific_bits);

        /*
         * (e: clean least important bit from porter)                   e = porter_aux | 0x01       e = porter_aux | 0x0F(00001111)
         */
        porter_aux |= signific_bits;

        /*
         * (f: combine and resultant byte)                              f = d & e
         * */
        info_aux &= porter_aux;
        return info_aux;
    }

    uint8_t read_bit(uint8_t info_aux, uint8_t porter_aux, uint8_t bit_l, size_t info_bit) {

        uint8_t signific_bits = std::pow(2,bit_l) - 1;

        info_aux <<= bit_l;
        porter_aux &= signific_bits;
        info_aux |= porter_aux;

        return info_aux;

    }

    std::string get_extension(const std::string filename){
        unsigned long last_dot_position = filename.find_last_of('.');
        return filename.substr(last_dot_position,filename.length()-1);
    }


    /*void steg::stegLSB(const char* porter_filename, const char* info_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

        std::FILE* porter_file = std::fopen(porter_filename,"rb");
        std::FILE* info_file = std::fopen(info_filename,"rb");

        std::string extension = get_extension(info_filename);
        const char* extension_c_str = extension.c_str();

        std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

        /*CONTROL SIZE 324686
        long porter_size = get_file_size(porter_file);
        auto info_size = (uint32_t) get_file_size(info_file);
        auto info_c_size = steg_f.get_info_c_size(info_size,extension.length(),steg_f.buffer_size);

        if (steg_f.compare_sizes(porter_size,info_size)){
            //TODO ERROR
            cerr << "INFO TOO BIG" << endl;
            exit(1);
        }

        auto porter_buffer = (uint8_t *) malloc(sizeof(uint8_t) * steg_f.buffer_size);
        uint8_t* info_buffer;
        auto c_info_buffer = (uint8_t*) malloc(sizeof(uint8_t) * steg_f.buffer_size);

        auto tmp_size_buffer = (uint8_t*) malloc(sizeof(uint32_t));
        memcpy(tmp_size_buffer,&info_size,4);

        bool pre_size_written = false;
        bool size_written = false;
        bool end = false;
        bool more_info = true;

        size_t porter_read = fread(porter_buffer,1,54,porter_file);
        size_t porter_i = porter_read;
        fwrite(porter_buffer, sizeof(uint8_t),54,destiny_file);

        size_t info_i = 0;
        size_t info_bit = 0;
        size_t info_read = 0;
        size_t pre_size_read = 0;
        size_t size_read = 0;
        size_t info_written = 0;
        size_t rrr = 0;

        size_t extension_read = 0;
        size_t written = 0;

        porter_buffer = (uint8_t*) malloc(sizeof(uint8_t) * steg_f.buffer_size);

        while (!end) {
            if (porter_i == porter_read) {
                porter_i = 0;
                porter_read = fread(porter_buffer, 1, steg_f.buffer_size, porter_file);
                rrr+=porter_read;
            }
            if (info_i == info_read && more_info) {
                info_i = 0;
                info_bit = 0;
                info_read = 0;
                info_buffer = (uint8_t*) malloc(sizeof(uint8_t) * steg_f.buffer_size);

                while (info_read < steg_f.buffer_size and more_info){
                    if (!pre_size_written){
                        if (pre_size_read != 4 && info_c_size != info_size){
                            if (steg_f.buffer_size - info_read > 4 - pre_size_read){
                                std::memcpy(info_buffer+info_read,(&info_c_size) + pre_size_read, 4 - pre_size_read);
                                info_read += 4 - pre_size_read;
                                pre_size_read = 4;
                            }
                            else{
                                std::memcpy(info_buffer+info_read,(&info_c_size) + pre_size_read, steg_f.buffer_size - info_read);
                                pre_size_read += steg_f.buffer_size - info_read;
                                info_read += steg_f.buffer_size - info_read;
                            }
                        }else{
                            break;
                        }
                    } else if (!size_written){

                        if (size_read != 4){
                            if (steg_f.buffer_size - info_read >= 4 - size_read){
                                //std::memcpy(info_buffer+info_read,(&info_size) + size_read, 4 - size_read);
                                std::memcpy(info_buffer+info_read,tmp_size_buffer + size_read, 4 - size_read);
                                info_read += 4 - size_read;
                                size_read = 4;
                            }
                            else{
                                //std::memcpy(info_buffer+info_read,(&info_size) + size_read, steg_f.buffer_size - info_read);
                                std::memcpy(info_buffer+info_read,tmp_size_buffer + size_read, steg_f.buffer_size - info_read);
                                size_read += steg_f.buffer_size - info_read;
                                info_read += steg_f.buffer_size - info_read;
                            }
                        }else{
                            size_written = true;
                        }

                    } else if (ftell(info_file) != info_size) {
                        info_read += fread(info_buffer+info_read, 1, steg_f.buffer_size-info_read, info_file);
                    } else if (extension.length() > info_written + info_read - info_size - 4) {
                        size_t i = 0;
                        if (steg_f.buffer_size >= info_read + extension.length() - extension_read) {
                            i = extension.length() - extension_read;
                        } else {
                            i = steg_f.buffer_size - info_read;
                        }
                        std::memcpy(info_buffer + info_read, extension_c_str + extension_read,i);
                        info_read += i;
                        extension_read += i;
                    } else if (extension.length() == info_written + info_read - info_size - 4) {
                        info_buffer[info_read] = '\0';
                        info_read += 1;
                        break;
                    }else {
                        more_info = false;
                    }
                }
                if(!pre_size_written){
                    pre_size_written = true;
                    if (info_c_size != info_size)
                        memcpy(c_info_buffer,info_buffer,4);
                    else
                        c_info_buffer = (steg_f.f)(info_buffer, steg_f.buffer_size,steg_f.data);
                } else if(more_info) {
                    c_info_buffer = (steg_f.f)(info_buffer, steg_f.buffer_size,steg_f.data);
                }
            }

            uint8_t y;
            uint8_t info_aux = c_info_buffer[info_i], porter_aux = porter_buffer[porter_i];
            if (more_info && ( !is_lsbe || (porter_aux&0xFE) == 0xFE)) {

                y = add_bit(info_aux,porter_aux,bit_l,info_bit);
                info_bit += bit_l;
                if (info_bit == 8){
                    info_bit = 0;
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

        fclose(destiny_file);
        fclose(porter_file);
        //TODO breaks in fclose
        // fclose(info_file);

        if(more_info){
            //TODO ERROR
            cerr << "INFO DO NOT ENTER, MAX SIZE = " << info_written << endl;
            exit(1);
        }
    }*/

void steg::stegLSB(const char* porter_filename, const char* info_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

    /*open IN files*/
    std::FILE* porter_file = std::fopen(porter_filename,"rb");
    std::FILE* info_file = std::fopen(info_filename,"rb");

    /*get extension of file*/
    std::string extension = get_extension(info_filename);
    const char* extension_c_str = extension.c_str();

    /*open OUT files*/
    std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

    /*CONTROL SIZES*/
    long porter_size = get_file_size(porter_file);
    auto info_size = (uint32_t) get_file_size(info_file);
    auto info_c_size = steg_f.get_info_c_size(info_size,extension.length(),steg_f.buffer_size);

    if (steg_f.compare_sizes(porter_size,info_size)){
        //TODO ERROR
        cerr << "INFO TOO BIG" << endl;
        exit(1);
    }

    /*Create buffers*/
    auto porter_buffer = (uint8_t *) malloc(sizeof(uint8_t) * porter_size);
    auto info_buffer = (uint8_t *) malloc(sizeof(uint8_t) * (steg::size_without_padding(info_size,extension.length(),0)));
    auto c_info_buffer = (uint8_t*) malloc(sizeof(uint8_t) * info_c_size + 4 /*Maybe necesary for size of compress*/);

    /*while condition var*/
    bool end = false;

    //Read file to buffer
    fread(porter_buffer,1,porter_size,porter_file);
    //write header
    fwrite(porter_buffer, sizeof(uint8_t),54,destiny_file);
    //Move index after header
    size_t porter_i = 54;

    //Info index
    size_t info_i = 0;
    //Bit index (within info byte
    size_t info_bit = 0;
    //Read control
    size_t info_read = 0;

    //size_t written = 0;


    size_t pre_size_written_i = 0;
    if(!steg_f.is_plain){
        memcpy(c_info_buffer,&info_c_size,4);
        pre_size_written_i += 4;
    }
    memcpy(info_buffer,&info_size,4);
    while(ftell(info_file) != info_size) {
        info_read += fread(info_buffer + 4 + info_read, 1, info_size - info_read, info_file);
    }
    memcpy(info_buffer+4+info_size,extension_c_str,extension.length());
    size_t i = steg::size_without_padding(info_size,extension.length(),0)-1;
    do{
        info_buffer[i] = 0;
        i++;
    }while (info_c_size >= i);

    info_buffer = steg_f.f(info_buffer,info_c_size, steg_f.data);
    memcpy(c_info_buffer + pre_size_written_i, info_buffer, info_c_size );

    while (!end) {
        uint8_t y;
        uint8_t info_aux = c_info_buffer[info_i], porter_aux = porter_buffer[porter_i];
        if (info_i <= info_c_size + pre_size_written_i && ( !is_lsbe || (porter_aux&0xFE) == 0xFE)) {

            y = add_bit(info_aux,porter_aux,bit_l,info_bit);
            info_bit += bit_l;
            if (info_bit == 8){
                info_bit = 0;
                info_i++;
            }
        } else {
            y = porter_aux;
        }


        //written +=
        fwrite(&y, sizeof(uint8_t),1,destiny_file);
        end = porter_i == porter_size;
        porter_i++;
    }

    fclose(destiny_file);
    fclose(porter_file);
    //TODO breaks in fclose
    // fclose(info_file);

    if(info_i < info_c_size){
        //TODO ERROR
        cerr << "INFO DO NOT ENTER, MAX SIZE = " << info_i << endl;
        exit(1);
    }
}

/*
void steg::dec_stegLSBB(const char* porter_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

    std::FILE* porter_file = std::fopen(porter_filename,"rb");
    std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

    long porter_size = get_file_size(porter_file);

    auto tmp_porter_buffer = (uint8_t *) malloc(sizeof(char) * 54);
    auto porter_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);
    auto size_buffer = (uint8_t *) malloc(sizeof(char) * 4);

    /*********auto info_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);
    /*********auto dec_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);

    size_t size_i = 0;
    size_t size_c_i = 0;

    size_t porter_read = fread(tmp_porter_buffer,1,54,porter_file);

    size_t info_bit = 0;

    size_t file_size = 0;
    size_t file_c_size = 0;
    size_t written = 0;

    bool size_c_read = steg_f.is_plain;

    bool end = false;
    uint8_t to_write_byte;
    uint8_t info_aux = 0;
    /*********uint8_t buffer_index = 0;

    string extension;

    while (!end) {
        porter_read = fread(porter_buffer, 1, 1, porter_file);

        uint8_t porter_aux = porter_buffer[0];
        if (( !is_lsbe || (porter_aux&0xFE) == 0xFE)) {

            info_aux = read_bit(info_aux,porter_aux,bit_l,info_bit);
            info_bit += bit_l;
            if (info_bit == 8){
                info_bit = 0;
                if (not steg_f.is_plain and size_c_i < 4) {
                    size_buffer[size_c_i] = info_aux;
                    size_c_i++;
                    if(size_c_i == 4){
                        file_c_size = *((uint32_t *) size_buffer);
                        size_c_read = true;
                        size_buffer = (uint8_t *) malloc(sizeof(char) * 4);
                    }
                } else {
                    memcpy(info_buffer+buffer_index,&info_aux,1);
                    buffer_index++;
                }
                if(size_c_read and buffer_index == steg_f.buffer_size){
                    buffer_index = 0;
                    int dec_index = 0;
                    dec_buffer = (steg_f.f)(info_buffer,steg_f.buffer_size,steg_f.data);
                    while (dec_index != steg_f.buffer_size) {
                        to_write_byte = *(dec_buffer+dec_index);
                        if (size_i < 4) {
                            std::memcpy((void *) (size_buffer + size_i), &to_write_byte, sizeof(uint8_t));
                            size_i++;
                            if (size_i == 4) {
                                file_size = *((uint32_t *) size_buffer);
                            }
                        } else if (written < file_size) {
                            written += fwrite(&to_write_byte, sizeof(uint8_t), 1, destiny_file);
                        } else {
                            end = to_write_byte == 0;
                            extension += to_write_byte;
                        }
                        dec_index++;
                    }
                }
                info_aux = 0;
            }
        }
    }

    fclose(porter_file);
    fclose(destiny_file);

    std::string dest = destiny_filename;
    dest += extension;

    rename(destiny_filename,dest.c_str());

}*/

void steg::dec_stegLSB(const char* porter_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

        std::FILE* porter_file = std::fopen(porter_filename,"rb");
        std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

        long porter_size = get_file_size(porter_file);

        auto tmp_porter_buffer = (uint8_t *) malloc(sizeof(char) * 54);
        auto porter_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);
        auto size_buffer = (uint8_t *) malloc(sizeof(char) * 4);

        /*********/auto info_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);
        /*********/auto dec_buffer = (uint8_t *) malloc(sizeof(char) * steg_f.buffer_size);

        size_t size_i = 0;
        size_t size_c_i = 0;

        size_t porter_read = fread(tmp_porter_buffer,1,54,porter_file);

        size_t info_bit = 0;

        size_t file_size = 0;
        size_t file_c_size = 0;
        size_t written = 0;

        size_t to_read_size = 0;

        bool size_c_read = steg_f.is_plain;
        /***********/uint8_t* to_read_buffer;

        bool end = false;
        bool extension_read = false;
        uint8_t to_write_byte;
        uint8_t info_aux = 0;
        /*********/size_t buffer_index = 0;

        string extension;



        while (!end) {
            porter_read = fread(porter_buffer, 1, 1, porter_file);

            uint8_t porter_aux = porter_buffer[0];
            if ((!is_lsbe || (porter_aux & 0xFE) == 0xFE)) {

                info_aux = read_bit(info_aux, porter_aux, bit_l, info_bit);
                info_bit += bit_l;
                if (info_bit == 8) {
                    info_bit = 0;
                    if (size_c_i < 4) {
                        size_buffer[size_c_i] = info_aux;
                        size_c_i++;
                        if (size_c_i == 4) {
                            to_read_size = *((uint32_t *) size_buffer);
                            to_read_buffer = (uint8_t *) malloc(sizeof(uint8_t) * to_read_size);
                            size_c_read = true;
                            size_buffer = (uint8_t *) malloc(sizeof(char) * 4);
                        }
                    } else {
                        if (steg_f.is_plain and buffer_index > to_read_size){
                            to_read_buffer = (uint8_t*) realloc(to_read_buffer,(buffer_index+1)* sizeof(uint8_t));
                            extension_read = info_aux == 0;
                        }

                        memcpy(to_read_buffer + buffer_index, &info_aux, 1);
                        buffer_index++;
                    }
                    info_aux = 0;
                }
            }
            if(steg_f.is_plain){
                end = to_read_size != 0 and buffer_index > to_read_size and extension_read;
            }else {
                end = to_read_size != 0 and buffer_index > to_read_size;
            }
        }
        if(steg_f.is_plain){
            info_buffer = to_read_buffer;
            file_size = to_read_size;
        }else{
            info_buffer = steg_f.f(to_read_buffer,to_read_size,steg_f.data);
            file_size = *((uint32_t *) info_buffer);
            info_buffer += 4;
        }
        fwrite(info_buffer,file_size,1,destiny_file);
        auto a = (char*) (info_buffer + file_size);


        fclose(porter_file);
        fclose(destiny_file);

        std::string dest = destiny_filename;
        dest += std::string(a);

        rename(destiny_filename,dest.c_str());

    }

    size_t steg::size_with_padding(size_t info_size, size_t extension_length, size_t buffer_size){
        size_t size = 4 + extension_length + 1 + info_size;
        size += (-size)%buffer_size;
        return size;
    }

    size_t steg::size_without_padding(size_t info_size, size_t extension_length, size_t buffer_size){
        return info_size + 4 + extension_length + 1;
    }

    bool steg::lsb1_size_compare(long porter_size, size_t info_size){
        return info_size * 8L > 3L * (porter_size - 53);
    }

    bool steg::lsb4_size_compare(long porter_size, size_t info_size){
        return info_size * 2L > 3L * (porter_size - 53);
    }

    bool steg::lsb8_size_compare(long porter_size, size_t info_size){
        return info_size * 1L > 3L * (porter_size - 53);
    }
