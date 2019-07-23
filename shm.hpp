#ifndef __SHM_HPP_
#define __SHM_HPP_  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <cstddef>

namespace sys_tools {

    #define DEFAULT_SHARED_MEMORY_NAME "defaultMemorySpaceName"
    #define DEFAULT_SHARED_MEMORY_SIZE (1024*1024)
    #define OPERATION_SUCCESSED 1
    #define OPERATION_FAILED 0

    bool not_null(void* ptr){
        return (ptr != nullptr && ptr != NULL);
    }

    class TSharedMemoryManager{

        public: 
            TSharedMemoryManager(const char* sharedMemoryName_, size_t sharedMemorySize_, int protection_, int visibility_):
                sharedMemorySize(sharedMemorySize_), protection(protection_), visibility(visibility_){                
                sharedMemoryName = nullptr;                                       
                sharedMemoryPtr = nullptr;                
                createSharedMemorySpace(sharedMemoryName_);
            }

            TSharedMemoryManager(const char* sharedMemoryName_, size_t sharedMemorySize_): TSharedMemoryManager( 
                sharedMemoryName_, 
                sharedMemorySize_, 
                PROT_READ | PROT_WRITE,  
                MAP_ANONYMOUS | MAP_SHARED){
            }

            TSharedMemoryManager(const char* sharedMemoryName_){
                
            }

            TSharedMemoryManager(): TSharedMemoryManager( 
                (char*) DEFAULT_SHARED_MEMORY_NAME, 
                DEFAULT_SHARED_MEMORY_SIZE, 
                PROT_READ | PROT_WRITE,  
                MAP_ANONYMOUS | MAP_SHARED){  
                // Memory buffer will be readable and writable.
                // The buffer will be shared (meaning other processes can access it), but
                // anonymous (meaning third-party processes cannot obtain an address for it),
                // so only this process and its children will be able to use it.            
            }

            ~TSharedMemoryManager(){
                removeSharedMemorySpace();
            }

            void* getMemoryPtr(){
                return sharedMemoryPtr;
            }        

            char* getMemoryName(){
                return sharedMemoryName;
            }

            void printInfo(){
                fprintf(stdout, "\nShared Memory\n - Space Name : \033[0;32m %s \033[0m\n - Size : \033[0;34m %d \033[0m\n - Buffer : \033[0;36m %s\n \033[0m", sharedMemoryName, (int) sharedMemorySize, (char*) sharedMemoryPtr);
            }
            /// size in bytes 
            int write(void* buffer, size_t size){
                if(sys_tools::not_null(buffer) && size <= sharedMemorySize){                    
                    memcpy(sharedMemoryPtr, buffer, size);
                    return OPERATION_SUCCESSED;
                }                    
                return OPERATION_FAILED;
            }

            void read(void** buffer){
                if(sys_tools::not_null(*buffer)) free(*buffer);
                *buffer = (void*) malloc(sharedMemorySize);
                memcpy(*buffer, sharedMemoryPtr, sharedMemorySize);
                fprintf(stdout, "\nbuffer : \n%s\n", (char*) *buffer);
            }

            void* getMemoryBuffer(){
                void* buffer = (void*) malloc(sharedMemorySize);
                memcpy(buffer, sharedMemoryPtr, sharedMemorySize);
                return buffer;
            }

        protected:
            
            void createSharedMemorySpace(const char* sharedMemoryName_){                                
                if(sys_tools::not_null(sharedMemoryName)){
                    free(sharedMemoryName);        
                }        
                sharedMemoryName = (char*) malloc(strlen(sharedMemoryName_) + 1);
                strcpy(sharedMemoryName, sharedMemoryName_);                
                /// Open physical memory 
                fd = shm_open((const char *)sharedMemoryName, O_RDWR, 0777);
                if (fd == -1) {   
                    fprintf(stderr, "Open failed:%s\n", strerror(errno));
                    return;                    
                }
                /// Map BIOS ROM
                sharedMemoryPtr = mmap(NULL, sharedMemorySize, protection, visibility, fd, 0xf0000);
                if(sharedMemoryPtr == (void *) -1){
                    fprintf(stderr, "mmap failed : %s\n", strerror(errno));
                    return;
                }
                printf("Map addr is %6.6X\n", sharedMemoryPtr);
            }

            void removeSharedMemorySpace(){
                munmap(sharedMemoryPtr, sharedMemorySize); 
                shm_unlink((const char *)sharedMemoryName); 
            }
        private:
            int fd;
            char* sharedMemoryName;
            size_t sharedMemorySize;
            void* sharedMemoryPtr;
            int protection;
            int visibility;
            
    };
}

#endif