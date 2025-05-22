#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "mapextract.h"

int read_raw_map(char *infile, map_t *map)
{
    FILE *inputRom = fopen(infile, "rb");
    if (inputRom == NULL) {
        printf("Failed to open rom file %s\n", infile);
        perror("fopen");
        return -1;
    }
    int len = map->length;
    long x_Address = (long)map->xAxis_address;
    long y_Address = (long)map->yAxis_address;
    long map_Address = (long)map->data_address;

    // set file pointer to x-Axis and read data
    fseek(inputRom, x_Address, SEEK_SET);
    if((fread(map->xAxis, 1, len, inputRom)) != (size_t)len){
        printf("Failed to read x-Axis from address %lX\n", x_Address);
        return -2;

    }

    // set file pointer to y-Axis and read data
    fseek(inputRom, y_Address, SEEK_SET);
    if((fread(map->yAxis, 1, len, inputRom)) != (size_t)len){
        printf("Failed to read y-Axis from address %lX\n", y_Address);
        return -3;

    }

    // set file pointer to map and read data
    fseek(inputRom, map_Address, SEEK_SET);
    if((fread(map->data, 1, len * len, inputRom)) != (size_t)(len*len)){
        printf("Failed to read map from address %lX\n", map_Address);
        return -4;
    }

    fclose(inputRom);
    return 0;   
}

int write_raw_map2file(char *outfile, map_t *map)
{
    FILE *outputMap = fopen(outfile, "w");
    if (outputMap == NULL) {
        printf("Failed to open output file %s\n", outfile);
        perror("fopen");
        return -1;
    }

    int len = map->length;
    
    int written_bytes = 0;
    char printLine[MAX_OUTPUTLINE_LENGTH];
    char *writePos = NULL;

    fprintf(outputMap, "x-Axis address: %X,\n", map->xAxis_address);
    fprintf(outputMap, "y-Axis address: %X,\n", map->yAxis_address);
    fprintf(outputMap, "map address: %X,\n\n", map->data_address);

    for(int row = -1; row < len; row++)
    {   
        memset(printLine, '\0', MAX_OUTPUTLINE_LENGTH);
        writePos = printLine;
        for(int column = -1; column < len; column++)
        {
            if(-1 == row)
            {   
                if(column < 0){
                    // leave a blank before the first x axis breakpoint
                    written_bytes = snprintf(writePos, MAX_OUTPUTLINE_LENGTH - (writePos - printLine), 
                                        ",");
                }else{
                    written_bytes = snprintf(writePos, MAX_OUTPUTLINE_LENGTH - (writePos - printLine), 
                                        "%3d,", map->xAxis[column]);
                }
            }else
            {   
                if(column < 0){
                     written_bytes = snprintf(writePos, MAX_OUTPUTLINE_LENGTH - (writePos - printLine), 
                            "%3d,", map->yAxis[row]);
                    
                }else{
                    written_bytes = snprintf(writePos, MAX_OUTPUTLINE_LENGTH - (writePos - printLine), 
                                        "%3d,", map->data[column + row*len]);
                }                
            }
            writePos += written_bytes;
            if(column == len -1){
                    // replace last comma with \n for newline
                    writePos--;
                    *writePos = '\n';
                    writePos++;
            }   
        }
        printf("%s", printLine);
        fputs(printLine, outputMap);
    }
    fclose(outputMap);

    return 0;
}

void init_map(map_t *map){
    if (!map) return;
    map->length = MAX_LENGTH;
    memset(map->data, 0, map->length * map->length);
    memset(map->xAxis, 0, map->length);
    memset(map->yAxis, 0, map->length);
    
    map->data_address = 0x0;
    map->xAxis_address = 0x0;
    map->yAxis_address = 0x0;
}