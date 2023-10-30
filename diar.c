#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* rebuildFile(FILE* part[], char filename[], int i){
    // continue opening the rest of the files
    // if another file is missing or corrupt then recovery is not possible
    // check to see that only one file is corrupt or missing
    // return the rebuilt file
    return NULL;
}

int main(int argc, char *argv[]){
    // assuming the user gives the correct command line arguments
    // assuming all partition files exist and raid has already been ran
    // try to open the 7 partiton files to read in binary
    FILE* part[7];
    char filename[50];
    for (int i = 0; i < 7; i++){
        sprintf(filename, "%s.part%d", argv[2], i);
        //printf("%s\n", filename);
        part[i] = fopen(filename, "rb");
        /*
        // if the file is null and its one of the data files we need to rebuild it using the other files
        if ((part[i] == NULL) && (i == 2 || i == 4 || i == 5 || i == 6)){ // possible corruption
            //part[i] = rebuildFile(part, filename, i);
            // no need to continue opening the files since they would have been opened in the function
            //break; // break out of the loop
        }
        else if((part[i] == NULL) && (i == 0 || i == 1 || i == 3)){
            // rebuilding may not be necessary but should be possible as long as no other files are corrupt
        }
        */
    }

    // now that all 7 partitons are open, use the four data parts to create a new file
    FILE* outFile;
    sprintf(filename, "%s.2", argv[2]);
    outFile = fopen(filename, "wb");
    
    // because you have to read each file by one byte each
    // each file should be the same size
    unsigned char uchar[4];
    unsigned char buffer;
    int arr[8];
    uchar[0] = fgetc(part[2]);
    uchar[1] = fgetc(part[4]);
    uchar[2] = fgetc(part[5]);
    uchar[3] = fgetc(part[6]);
    while (!feof(part[6]) || !feof(part[5]) || !feof(part[4]) || !feof(part[2])){
        // build an array
        for (int i = 0, j = 0, k = 7; 0 <= k; i++, j++){
            arr[i] = (uchar[j] >> k) & 0x01;
            if (j == 3){
                j = -1;
                k--;
            }
            if (i == 7){
                for (int i = 0; i < 8; i++) buffer = (buffer << 1) | arr[i];
                fwrite(&(buffer), 1, 1, outFile);
                i = -1;
            }
        }
        
        // read in the next byte from the parts
        uchar[0] = fgetc(part[2]);
        uchar[1] = fgetc(part[4]);
        uchar[2] = fgetc(part[5]);
        uchar[3] = fgetc(part[6]);
    }
    
    // close the files
    for (int i = 0; i < 7; i++) fclose(part[i]);
    fclose(outFile);

    return 0;
}