#ifndef __HCD_HPP_INCLUDED__   
#define __HCD_HPP_INCLUDED__   

#include <stdint.h>
#include <math.h>

namespace sys_tools{
    /// --------------------------------------------------------------------------------------------
    ///  64 ( 2^6 ) characters array
    ///  '-'  - minus - 45
    ///  48 to 57
    ///  65 to 94
    ///  '_'  - low line - 95
    ///  97 to 122
    /// --------------------------------------------------------------------------------------------
    void generate_ABC(char* ABC){ 
        int i = 0;
        ABC[i++] = '-';       
        for(char c = '0'; c <= '9'; c++) ABC[i++] = c;    
        for(char c = 'A'; c <= 'Z'; c++) ABC[i++] = c;
        ABC[i++] = '_';
        for(char c = 'a'; c <= 'z'; c++) ABC[i++] = c;
    }

    char get_ABC(unsigned short i){
        if(i == 0) return '-';
        if(i>=1 && i<=10) return '0' + (i - 1);
        if(i>=11 && i<=36) return 'A' + (i - 11);
        if(i == 37) return '_';
        if(i>=38 && i<=63) return 'a' + (i - 38);    
    }

    unsigned short get_index_ABC(char c){        
        if(c>=65 && c<=90) return (unsigned short)(c - 54);    
        if(c>=97 && c<=122) return (unsigned short)(c - 59);
        if(c>=48 && c<=57) return (unsigned short)(c - 47);
        if(c == 95) return 37;
        return 0;
    }
    /// --------------------------------------------------------------------------------------------
    /// hex_encode - to encode data into 64 char alphabet 
    /// original - points on memory with data
    /// N - memory size of original data
    /// buffer - points on memory to store encoded data
    /// --------------------------------------------------------------------------------------------
    void hex_encode(unsigned char* original, unsigned int N, unsigned char* buffer){            
        uint64_t* ptr = (uint64_t*) original; 
        unsigned int K = (8 * sizeof(uint64_t)) / 6;
        unsigned int M =  ceil((float)N / (float)sizeof(uint64_t)) ;
        uint64_t hex_mask = 0;
        hex_mask = ~((~hex_mask) << 6); // 6 bit mask          

        for(int i = 0; i < M; i++){
            uint64_t v = *(ptr+i);        
            for(int j = 0; j < K; j++){
                *(buffer + (K+1)*i + j) = get_ABC(v & hex_mask);
                v = v >> 6;   
            }
            uint64_t m = 15; // 4 bit mask
            *(buffer + (K+1)*i + K) = get_ABC(v & m);
        }        
    }
    /// --------------------------------------------------------------------------------------------
    /// hex_decode - to encode data into 64 char alphabet 
    /// encoded - points on memory where encoded data is stored
    /// N - memory size of decoded data
    /// buffer - points on memory to store decoded data
    /// --------------------------------------------------------------------------------------------
    void hex_decode(unsigned char* encoded, unsigned int N, unsigned char* buffer){    
        uint64_t* ptr = (uint64_t*) buffer;    
        unsigned int K = (8 * sizeof(uint64_t)) / 6;
        unsigned int M =  ceil((float)N / (float)sizeof(uint64_t)) ;
        uint64_t hex_mask = 0;
        hex_mask = ~((~hex_mask) << 6); // 6 bit mask          
        for(int i = 0; i < M; i++){
            uint64_t v = 0;
            for(int j = 0; j <= K; j++){
                v = v << 6;            
                v = v & (~hex_mask);
                v += get_index_ABC(*(encoded+(K+1)*(i+1) - j - 1));               
            }
            *(ptr + i) = v;
        }
    }

}

#endif