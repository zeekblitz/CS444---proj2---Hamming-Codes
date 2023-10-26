#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE* inFile;
    // assuming the user gives the correct command line arguments
    // try to open an existing file to read in
    file = fopen(argv[2], "r");
    if (!file){
        printf("%s does not exist.\n", argv[2]);
        return -1;
    }

    // create 7 files to write to
    FILE* outFile[7];
    char filename[50];
    for (int i = 0; i < 7; i++){
        sprintf(filename, "%s.part%d", argv[2], i);
        //printf("%s\n", filename);
        outFile[i] = fopen(filename, "wb");
    }

    // read the input file byte by byte (char by char)
    // then nibble by nibble create a byte with 3 parity bits
    
}