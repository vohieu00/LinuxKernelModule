#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define SIZE	4
#define FILENAME	"/dev/generateDevice"
int main(){
    int ret, folder;
    char numReceive[SIZE];
    printf("Starting device test code example...\n");
    
    folder = open(FILENAME, O_RDWR); //Open the device to read
    if (folder < 0){
        perror("Failed to open the device..\n");
        return errno;
    }
    printf("Reading from the device...\n"); //Read number from generator
    ret = read(folder, numReceive, SIZE);
    if (ret < 0){
        perror("Failed to read from the device...\n");
        return errno;
    }

    printf("A received number is %u", numReceive[0]);
    
    printf("\nEnd of the program\n");
    close(folder);
    return 0;
}
