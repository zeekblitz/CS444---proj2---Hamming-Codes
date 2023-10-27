#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct byteBuffer{
    unsigned char buffer;
} ByteBuffer;

FILE* rebuildFile(FILE* part[], char* curruptFileName, int i){
    // continue opening the rest of the files
    // if another file is missing or corrupt then recovery is not possible
    // check to see that only one file is corrupt or missing
    // return the rebuilt file
    return NULL;
}

int main(int argc, char *argv[]){
    FILE* inFile;
    // assuming the user gives the correct command line arguments
    // assuming all partition files exist and raid has already been ran
    // try to open the 7 partiton files to read in binary
    FILE* part[7];
    char filename[50];
    for (int i = 0; i < 7; i++){
        sprintf(filename, "%s.part%d", argv[2], i);
        //printf("%s\n", filename);
        part[i] = fopen(filename, "rb");
        // if the file is null and its one of the data files we need to rebuild it using the other files
        if (part[i] == NULL && i == 2 || i == 4 || i == 5 || i == 6){ // possible corruption
            part[i] = rebuildFile()
            // no need to continue opening the files since they would have been opened in the function
            break;
        }
        else if(part[i] == NULL && i == 0 || i == 1 || i == 3){
            // rebuilding may not be necessary but should be possible as long as no other files are corrupt
            // 
        }

        // now that all 7 partitons are open...

    }

    for (int i = 0; i < 7; i++) fclose(part[i]);
}