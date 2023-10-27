#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct byteBuffer{
    unsigned char buffer;
} ByteBuffer;

int main(int argc, char *argv[]){
    FILE* inFile;
    // assuming the user gives the correct command line arguments
    // try to open an existing file to read in
    inFile = fopen(argv[2], "r");
    if (!inFile){
        printf("%s does not exist.\n", argv[2]);
        return -1;
    }

    // create 7 files to write to in binary
    FILE* outFile[7]; // files 0, 1, and 3 are for storing parity bits
    char filename[50];
    for (int i = 0; i < 7; i++){
        sprintf(filename, "%s.part%d", argv[2], i);
        //printf("%s\n", filename);
        outFile[i] = fopen(filename, "wb");
    }

    // read the input file byte by byte (char by char) until the end
    char ch;
    int arr[8], count = 0, bytes = 0;
    ByteBuffer bb[7]; // 7 bytes for each file
    ch = fgetc(inFile);
    while (ch != EOF){
        // update the number of bytes in the file
        bytes++;
        // create an array of bits based on the char
        for (int i = 0, j = 7; 0 <= j; i++, j--){
            arr[i] = (ch >> j) & 0x01;
            //printf("%d", arr[i]);
        }

        // I tried to make this part into a loop
        // but the logic was getting out of hand,
        // and it would have been the same number of lines
        bb[2].buffer = (bb[2].buffer << 1) | arr[0];
        bb[4].buffer = (bb[4].buffer << 1) | arr[1];
        bb[5].buffer = (bb[5].buffer << 1) | arr[2];
        bb[6].buffer = (bb[6].buffer << 1) | arr[3];

        // manage the parity bits
        // I could make this part into fewer lines, 
        // but it would make it more unreadable than it already is
        if (((arr[0] + arr[1] + arr[3]) % 2) == 0) // even parity
        bb[0].buffer = (bb[0].buffer << 1) | 0;
        else bb[0].buffer = (bb[0].buffer << 1) | 1; // odd parity
        if (((arr[0] + arr[2] + arr[3]) % 2) == 0) // even parity
        bb[1].buffer = (bb[1].buffer << 1) | 0;
        else bb[1].buffer = (bb[1].buffer << 1) | 1; // odd parity
        if (((arr[1] + arr[2] + arr[3]) % 2) == 0) // even parity
        bb[3].buffer = (bb[3].buffer << 1) | 0;
        else bb[3].buffer = (bb[3].buffer << 1) | 1; // odd parity

        // update the count
        count++;

        /*----------------------------------------------------------*/

        //do it again for the second nibble
        bb[2].buffer = (bb[2].buffer << 1) | arr[4];
        bb[4].buffer = (bb[4].buffer << 1) | arr[5];
        bb[5].buffer = (bb[5].buffer << 1) | arr[6];
        bb[6].buffer = (bb[6].buffer << 1) | arr[7];

        // manage the parity bits again
        if (((arr[4] + arr[5] + arr[7]) % 2) == 0) // even parity
        bb[0].buffer = (bb[0].buffer << 1) | 0;
        else bb[0].buffer = (bb[0].buffer << 1) | 1; // odd parity
        if (((arr[4] + arr[6] + arr[7]) % 2) == 0) // even parity
        bb[1].buffer = (bb[1].buffer << 1) | 0;
        else bb[1].buffer = (bb[1].buffer << 1) | 1; // odd parity
        if (((arr[5] + arr[6] + arr[7]) % 2) == 0) // even parity
        bb[3].buffer = (bb[3].buffer << 1) | 0;
        else bb[3].buffer = (bb[3].buffer << 1) | 1; // odd parity

        // update the count again
        count++;

        // if the bytebuffer contains 8 bits write it to its relevant file
        if (count % 8 == 0){
            // at this point all 7 ByteBuffers should contain 8 bits = 1 byte
            for (int i = 0; i < 7; i++){
                fwrite(&(bb[i].buffer), 1, 1, outFile[i]);

                // clear the buffer
                bb[i].buffer = 0;
            }
            
            // reset the bit counter to 0
            count = 0;
            // update the bytes counter
        }

        ch = fgetc(inFile);
    }

    // fill any remaining bits with 0's
    if (count > 0){
        while(count != 8){
            for (int i = 0; i < 7; i++) bb[i].buffer = (bb[i].buffer << 1) | 0;
            count++;
        }
        for (int i = 0; i < 7; i++) fwrite(&(bb[i].buffer), 1, 1, outFile[i]);
    }

    //close all of the files
    fclose(inFile);
    for (int i = 0; i < 7; i++) fclose(outFile[i]);
    
    // print the filename and number of bytes
    printf("%s: %d B\n", argv[2], bytes);

    return 0;
}