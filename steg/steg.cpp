//
// Created by eric on 10/05/18.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "steg.h"

#include <byteswap.h>
#include <openssl/aes.h>

using namespace std;

    bool (*steg::compare_sizes_lsbe [])(long,size_t) = {steg::lsb1_size_compare,steg::lsb4_size_compare,steg::lsb1_size_compare,steg::lsb8_size_compare};
    bool steg::is_lsbe[] = {false, false, true, false};
    uint8_t steg::lsb_bits[] = {1, 4, 1, 8};

    bool control_header(uint8_t* header, long file_size) {
        /*
         * 0x42 0x4D + BM
         * Bytes
         * 0, 1	            "BM" file type
         * 2, 3, 4, 5	    File size
         * 6, 7	            Reserved
         * 8, 9	            Reserved
         * 10, 11, 12, 13	Picture data begin
         * 14, 15, 16, 17	Bitmap header size
         * 18, 19, 20, 21	Length (pixels)
         * 22, 23, 24, 25	Height (pixels)
         * 26, 27	        Plane number
         * 28, 29	        Pixel size
         * 30, 31, 32, 33	Compressed (0=no compress)
         * 34, 35, 36, 37	Picture size
         * 38, 39, 40, 41	Horizontal resolution
         * 42, 43, 44, 45	Vertical resolution
         * 46, 47, 48, 49	Palette size
         * 50, 51, 52, 53	Important colors count
         */
        bool error = false;
        if(header[0] != 0x42 or header[1] != 0x4D){
            std::cerr << "HEAD_ERROR: Not begin with BM" << std::endl;
            error = true;
        }
        int header_file_size = *((uint32_t*)(header + 2));
        if( header_file_size != file_size){
            std::cerr << "HEAD_ERROR: Header file size indicated differ from file size" << std::endl;
            //error = true;
        }
        int image_start = *((uint32_t*)(header + 10));
        if (image_start != HEADER_LENGTH){
            std::cerr << "HEAD_ERROR: Header length is not 54" << std::endl;
            error = true;
        }
        int header_size = *((uint32_t*)(header + 14));
        if (header_size != HEADER_SIZE){
            std::cerr << "HEAD_ERROR: Header size is not 40" << std::endl;
            error = true;
        }
        int bits_per_pixel = *((uint16_t*)(header + 28));
        if (bits_per_pixel != BITS_PER_PIXEL){
            std::cerr << "HEAD_ERROR: It is not 24 bits per pixel" << std::endl;
            error = true;
        }
        int compression_mode = *((uint32_t*)(header + 30));
        if (compression_mode != COMPRESS_MODE){
            std::cerr << "HEAD_ERROR: The file must not be compressed" << std::endl;
            error = true;
        }
        return error;
    }

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

void steg::stegLSB(const char* porter_filename, const char* info_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

    /*open IN files*/
    std::FILE* porter_file = std::fopen(porter_filename,"rb");
    std::FILE* info_file = std::fopen(info_filename,"rb");


    if (porter_file == nullptr){
        cerr << "Can't open porter file"<< endl;
        exit(1);
    }
    if (info_file == nullptr){
        cerr << "Can't open in file"<< endl;
        fclose(porter_file);
        exit(1);
    }

    /*get extension of file*/
    std::string extension = get_extension(info_filename);
    const char* extension_c_str = extension.c_str();

    /*open OUT files*/
    std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

    if (destiny_file == nullptr){
        cerr << "Can't open out file"<< endl;
        fclose(porter_file);
        fclose(info_file);
        exit(1);
    }

    /*CONTROL SIZES*/
    long porter_size = get_file_size(porter_file);
    auto info_size = (uint32_t) get_file_size(info_file);
    auto info_c_size = steg_f.get_info_c_size(info_size,extension.length(),steg_f.buffer_size);

    if (steg_f.compare_sizes(porter_size,info_size)){
        cerr << "INPUT FILE SIZE TOO BIG" << endl;
        fclose(porter_file);
        fclose(destiny_file);
        remove(destiny_filename);
        exit(1);
    }

    /*Create buffers*/
    auto porter_buffer = (uint8_t *) malloc(sizeof(uint8_t) * porter_size);
    auto info_buffer = (uint8_t *) malloc(sizeof(uint8_t) * (steg::size_without_padding(info_size,extension.length(),0)));
    auto c_info_buffer = (uint8_t*) malloc(sizeof(uint8_t) * info_c_size + SIZE_BYTES /*Maybe necesary for size of compress*/);

    /*while condition var*/
    bool end = false;

    //Read file to buffer
    fread(porter_buffer,1,porter_size,porter_file);
    //write header
    fwrite(porter_buffer, sizeof(uint8_t),HEADER_LENGTH,destiny_file);
    if (control_header(porter_buffer, porter_size)){
        fclose(porter_file);
        fclose(destiny_file);
        remove(destiny_filename);
        exit(1);
    }
    //Move index after header
    size_t porter_i = HEADER_LENGTH;

    //Info index
    size_t info_i = 0;
    //Bit index (within info byte
    size_t info_bit = 0;
    //Read control
    size_t info_read = 0;

    size_t pre_size_written_i = 0;
    if(!steg_f.is_plain){
        int info_c_size_big = __bswap_32 (info_c_size);
        memcpy(c_info_buffer,&info_c_size_big,SIZE_BYTES);
        pre_size_written_i += SIZE_BYTES;
    }
    int info_size_big = __bswap_32(info_size);
    memcpy(info_buffer,&info_size_big,SIZE_BYTES);
    while(ftell(info_file) != info_size) {
        info_read += fread(info_buffer + SIZE_BYTES + info_read, 1, info_size - info_read, info_file);
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

        fwrite(&y, sizeof(uint8_t),1,destiny_file);
        porter_i++;
        end = porter_i == porter_size;
    }

    fclose(destiny_file);
    fclose(porter_file);
    fclose(info_file);

    if(info_i < info_c_size){
        cerr << "INPUT FILE DO NOT ENTER, MAX SIZE = " << info_i << endl;
        exit(1);
    }
}

void steg::dec_stegLSB(const char* porter_filename, const char* destiny_filename, const uint8_t bit_l, const bool is_lsbe, steg::steg_function steg_f) {

        std::FILE* porter_file = std::fopen(porter_filename,"rb");
        std::FILE* destiny_file = std::fopen(destiny_filename,"wb");

        if (porter_file == nullptr){
            cerr << "Can't open porter file"<< endl;
            exit(1);
        }
        if (destiny_file == nullptr){
            cerr << "Can't open out file"<< endl;
            fclose(porter_file);
            exit(1);
        }

        long porter_size = get_file_size(porter_file);

        auto tmp_porter_buffer = (uint8_t *) malloc(sizeof(uint8_t) * HEADER_LENGTH);
        auto porter_buffer = (uint8_t *) malloc(sizeof(uint8_t));
        auto size_buffer = (uint8_t *) malloc(sizeof(uint8_t) * SIZE_BYTES);

        uint8_t * info_buffer;

        size_t size_c_i = 0;

        fread(tmp_porter_buffer,1,HEADER_LENGTH,porter_file);
        if (control_header(tmp_porter_buffer,porter_size)){
            fclose(porter_file);
            fclose(destiny_file);
            remove(destiny_filename);
            exit(1);
        }

        size_t info_bit = 0;

        size_t file_size = 0;

        size_t to_read_size = 0;

        uint8_t* to_read_buffer;

        bool end = false;
        bool extension_read = false;
        uint8_t info_aux = 0;
        size_t buffer_index = 0;

        string extension;

        while (!end) {
            fread(porter_buffer, 1, 1, porter_file);

            uint8_t porter_aux = porter_buffer[0];
            if ((!is_lsbe || (porter_aux & 0xFE) == 0xFE)) {

                info_aux = read_bit(info_aux, porter_aux, bit_l, info_bit);
                info_bit += bit_l;
                if (info_bit == 8) {
                    info_bit = 0;
                    if (size_c_i < SIZE_BYTES) {
                        size_buffer[size_c_i] = info_aux;
                        size_c_i++;
                        if (size_c_i == SIZE_BYTES) {
                            to_read_size = *((uint32_t *) size_buffer);
                            to_read_size = __bswap_32(to_read_size);
                            to_read_buffer = (uint8_t *) malloc(sizeof(uint8_t) * to_read_size);
                            if(to_read_size >= porter_size - (HEADER_LENGTH + SIZE_BYTES) /*4 of size*/){
                                std::cerr << "ERR: Cipher data size is greater than porter file size" << std::endl;
                                fclose(porter_file);
                                fclose(destiny_file);
                                remove(destiny_filename);
                                exit(1);
                            }
                            if( to_read_size <= 0){
                                std::cerr << "ERR: Cipher data size is less or equal to 0" << std::endl;
                                fclose(porter_file);
                                fclose(destiny_file);
                                remove(destiny_filename);
                                exit(1);
                            }
                            size_buffer = (uint8_t *) malloc(sizeof(uint32_t));
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
            file_size = __bswap_32(file_size);
            info_buffer += SIZE_BYTES;
            if(file_size >= to_read_size - SIZE_BYTES){
                std::cerr << "ERR: Stenographer data size is greater than ciphered data size" << std::endl;
                fclose(porter_file);
                fclose(destiny_file);
                remove(destiny_filename);
                exit(1);
            }
        }
        fwrite(info_buffer,file_size,1,destiny_file);
        auto a = (char*) (info_buffer + file_size);

        fclose(porter_file);
        fclose(destiny_file);
        if(a[0] != '.'){
            std::cerr << "WARN: Extension do NOT start with '.' Taking the first 3 characters as extension" << std::endl;
            a[-1] = '.';
            a[4] = '\0';
            a = a - 1;
        }

        std::string dest = destiny_filename;
        dest += std::string(a);

        rename(destiny_filename,dest.c_str());

    }

    size_t steg::size_with_padding(size_t info_size, size_t extension_length, size_t buffer_size){
        size_t size = SIZE_BYTES + extension_length + 1 + info_size;
        size += (-size)%buffer_size;
        return size;
    }

    size_t steg::size_without_padding(size_t info_size, size_t extension_length, size_t buffer_size){
        return info_size + SIZE_BYTES + extension_length + 1;
    }

    bool steg::lsb1_size_compare(long porter_size, size_t info_size){
        return info_size * 8L > (porter_size - HEADER_SIZE);
    }

    bool steg::lsb4_size_compare(long porter_size, size_t info_size){
        return info_size * 2L > (porter_size - HEADER_SIZE);
    }

    bool steg::lsb8_size_compare(long porter_size, size_t info_size){
        return info_size * 1L > (porter_size - HEADER_SIZE);
    }
