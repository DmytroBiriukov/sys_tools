#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <shm.hpp>
#include <frk.hpp>
#include <struct_interface.hpp>
#include <prn.hpp>
#include <hcd.hpp>

/// Test shared memory
void test_shm(){
    sys_tools::TSharedMemoryManager sharedMemory("someNameForMemorySpace");    
    //char message[] = "Hello, shared memmory!";
    //sharedMemory.write((void*)message, strlen(message));    
    int message[] = {   ('d' << 24 ) + ('c' << 16) + ('b' << 8 ) + 'a', 
                        ('h' << 24 ) + ('g' << 16) + ('f' << 8 ) + 'e'
                    };    
    sharedMemory.write((void*)message, 2 * sizeof(int));
    
    sharedMemory.printInfo();
    void* buffer = nullptr; // assign to NULL is working as well    
    sharedMemory.read(&buffer);    
    if(sys_tools::not_null(buffer)) free(buffer);
}


/// Test fork process
void work_load(char* info, char* message, void* shm_mng_){
    sys_tools::TSharedMemoryManager* shm_mng = (sys_tools::TSharedMemoryManager*) shm_mng_;
    //printf("%s read: %s\n", info, (char*) shm_mng->getMemoryPtr());
    shm_mng->write(message, strlen(message) + 1);
    //printf("%s wrote: %s\n", info, (char*) shm_mng->getMemoryPtr());       
    fprintf(stdout, "\n Child created with pid %d and parent %d", (int) getpid(), (int) getppid());   
}

void test_frk(){
    /// Create memory buffer and fill it with some data
    sys_tools::TSharedMemoryManager sharedMemory("someNameForMemorySpace");    
    char message[] = "Hello, shared memmory!";
    sharedMemory.write((void*)message, strlen(message));
    /// Fork children and pass them work load and memory
    unsigned short childProcessesNumber = 4; 
    sys_tools::processDescription childProcessesInfo[childProcessesNumber];    
    sys_tools::forker(childProcessesInfo, (unsigned short) 0, childProcessesNumber, work_load, &sharedMemory);    
}

void test_struct_interface(){
    sys_tools::setter test_setter = sys_tools::setter(5, 7, "Hello my friend", ", hello again !");         
    fprintf(stdout, "\nSetter says :\n%s%s\n", test_setter.s1, test_setter.s2 );

    void* params[] = { (void*)&test_setter.i1, (void*)&test_setter.i2, (void*)test_setter.s1, (void*)test_setter.s2 };
    sys_tools::getter test_getter = sys_tools::getter(params); 
    fprintf(stdout, "\nGetter says :\n%s%s\n and interer values : %d and %d\n, again string values : %s %s ", 
        test_getter.s1(), test_getter.s2(), 
        test_getter.get_int("i1"), test_getter.get_int("i2"), 
        test_getter.get_str("s1"), test_getter.get_str("s2")
    );
}

void test_prn(){
    sys_tools::prnl("prefix", "message", "suffix");
}

/// Test hex encoding and decoding
/// param - is a multiplier of original array size
void test_hcd(unsigned int mult){     
    unsigned int N = mult * 160;
    unsigned char original_arr[N];
    for(int j=0; j < mult * 4; j++)
        for(int i=0; i<40; i++) original_arr[40*j + i] = 48 + i;  
    unsigned int K = (8 * sizeof(uint64_t)) / 6;
    unsigned int M =  ceil((float)N / (float)sizeof(uint64_t)) ;
    unsigned char buffer[(K+1) * M];  
    unsigned char decoded_arr[N];

    sys_tools::hex_encode(original_arr, N, buffer);    
    sys_tools::hex_decode(buffer, N, decoded_arr);

    for(int i=0; i < (K+1) * M; i++) fprintf(stdout, "%c", *(buffer+i)); 
    fprintf(stdout, "\n\n");    
    for(int i=0; i < N; i++) fprintf(stdout, "%c", *(original_arr+i)); 
    fprintf(stdout, "\n\n");
    for(int i=0; i < N; i++) fprintf(stdout, "%c", *(decoded_arr+i));     
    fprintf(stdout, "\n\n N = %d M = %d K = %d, SIZE = %d, SIZE/N = %2.3f\n\n", N, M, K, ((K+1) * M), (float)((K+1) * M) / (float)N);

    for(int i=0; i < N; i++){
        if(*(original_arr+i) != *(decoded_arr+i)){
            fprintf(stdout, "\nFAILED\n");
            break;
        }
    }   
}

int main(){
 fprintf(stdout, "\nHello from test");
 fprintf(stdout, "\nHello for Jenkins");
 return 0;
}
