
namespace sys_tools{

void timeNow(){
    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    printf("Current Time : %s\n", time_str);
    printf("Seconds since the Epoch : %d", mytime); 
}
}
