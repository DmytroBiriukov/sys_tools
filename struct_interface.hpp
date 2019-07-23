#ifndef __STRUCT_INTERFACE_HPP_INCLUDED__   // if x.h hasn't been included yet...
#define __STRUCT_INTERFACE_HPP_INCLUDED__   //   #define this so the compiler knows it has been included

/*
  This example demonstrates 
  how to keep pointers to some data
  and provide interface 
  through structure methods

*/
#include <string.h>
#include <stdlib.h>

namespace sys_tools{
    /// Example getter structure
    class getter{
    
        public:
            getter(void* ptrs[]){
                for(int i=0; i<4; i++) ptr[i] = ptrs[i];       
            }

            int get_int(const char* key){
                for(int i=0; i < 4; i++) {
                    if(strcmp(key, index[i]) == 0){
                        return *(int*)*(ptr+i);
                    }
                }
            }

            char* get_str(const char* key){
                for(int i=0; i < 4; i++) {
                    if(strcmp(key, index[i]) == 0){
                        return (char*)*(ptr+i);
                    }
                }
            }


            int i1(){ return *(int*)*ptr; };
            int i2(){ return *(int*)*(ptr+1); };
            char* s1(){ return (char*)*(ptr+2); };
            char* s2(){ return (char*)*(ptr+3); };
        
        private:
            char* index[4] = { "i1", "i2", "s1", "s2" };
            void * ptr[4];
    };

    struct setter{
        int i1, i2;
        char *s1, *s2;
        setter(int p1, int p2, char* p3, char* p4){
            i1 = p1; i2 = p2; 
            s1 = (char*) malloc(16); memcpy(s1, p3, 16);
            s2 = (char*) malloc(16); memcpy(s2, p4, 16);
        }
    };

}

#endif