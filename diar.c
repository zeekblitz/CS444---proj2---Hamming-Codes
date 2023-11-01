#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errorcheck(unsigned char uchar[]){
    int arr[7], error = 0;
    for (int i = 0, j = 7; j >= 0; i++){
        arr[i] = (uchar[i] >> j) & 0x01;
        if (i == 6){
            // the array is full now check the bits and flip any that are not correct
            if (arr[0]^arr[2]^arr[4]^arr[6]) (error += 1); // P1 ^ D1 ^ D2 ^ D4
            if (arr[1]^arr[2]^arr[5]^arr[6]) (error += 2); // P2 ^ D1 ^ D3 ^ D4
            if (arr[3]^arr[4]^arr[5]^arr[6]) (error += 4); // P4 ^ D2 ^ D3 ^ D4
            if (error > 0){
                uchar[error-1] = uchar[error-1] ^ (0x01 << j);
                error = 0; // i spent two hours debugging before i realized that i forgot to do this😭
            }
            // move to the next set of bits
            i = -1;
            j--;
        }
    }
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
        if (part[i] == NULL){
            printf("%s is missing.", filename);
            return -1;
        }
    }

    // now that all 7 partitons are open, use the four data parts to create a new file
    FILE* outFile;
    sprintf(filename, "%s.2", argv[2]);
    outFile = fopen(filename, "wb");
    
    // because you have to read each file by one byte each
    // each file should be the same size
    unsigned char uchar[7]; // to hold a single byte from each part
    unsigned char buffer; // byte buffer to be written to file
    int arr[8]; // array to hold the bits
    for (int i = 0; i < 7; i++) uchar[i] = fgetc(part[i]);
    for (int i = 0; i < (atoi(argv[4])/4); i++){
    //while (!feof(part[6]) && !feof(part[5]) && !feof(part[4]) && !feof(part[2])){ // <--------------------------
        // check for errors
        errorcheck(uchar);
        // build the byte array by taking 1 bit from each of the files, twice
        for (int k = 7; k >= 0;){
            arr[0] = (uchar[2] >> k) & 0x01;
            arr[1] = (uchar[4] >> k) & 0x01;
            arr[2] = (uchar[5] >> k) & 0x01;
            arr[3] = (uchar[6] >> k) & 0x01;
            k--;
            arr[4] = (uchar[2] >> k) & 0x01;
            arr[5] = (uchar[4] >> k) & 0x01;
            arr[6] = (uchar[5] >> k) & 0x01;
            arr[7] = (uchar[6] >> k) & 0x01;
            k--;
            // once you have 8 bits in the array, put them in the byte buffer
            for (int i = 0; i < 8; i++) buffer = (buffer << 1) | arr[i];
            fwrite(&(buffer), 1, 1, outFile); // write the byte buffer to the new file
        }
        
        // read in the next byte from the parts
        for (int i = 0; i < 7; i++) uchar[i] = fgetc(part[i]);
    }
    
    // close the files
    for (int i = 0; i < 7; i++) fclose(part[i]);
    fclose(outFile);

    return 0;
}