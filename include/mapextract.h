#ifndef MAPEXTRACT_H
#define MAPEXTRACT_H

#include <stdint.h>

#define MAX_LENGTH 255
#define MAX_OUTPUTLINE_LENGTH 2048

typedef struct {
    uint16_t xAxis_address;
    uint8_t xAxis[MAX_LENGTH];
    uint16_t yAxis_address;
    uint8_t yAxis[MAX_LENGTH];
    uint16_t data_address;
    uint8_t data[MAX_LENGTH*MAX_LENGTH];
    int length;
} map_t;

int read_raw_map(char *infile, map_t *map);
int write_raw_map2file(char *outfile, map_t *map);
void init_map(map_t *map);

#endif // MAPEXTRACT_H
