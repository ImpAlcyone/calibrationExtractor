#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "mapextract.h"


static int get_address(char *str);

void print_usage() {
    printf("Usage: mapextract [-i input_file] [-o output_file] [-m map_address] [-x x-Axis_address] [-y y-Axis_address] [-l length in bytes]\n");
}

int main(int argc, char *argv[])
{
	char opt;
    char *infile = NULL;
    char *outfile = NULL;
    map_t map;
    init_map(&map);
		
    if(argc < 7){
        print_usage();
    }

	while ((opt = getopt(argc, argv, "i:o:m:x:y:l:")) != -1 )
	{
		switch(opt)
		{
			case 'i': infile=optarg;
				  break;
			case 'o': outfile=optarg;
				  break;
			case 'm': map.data_address = get_address(optarg);
				  break;
            case 'x': map.xAxis_address = get_address(optarg);
				  break;
            case 'y': map.yAxis_address = get_address(optarg);
				  break;
            case 'l': map.length = atoi(optarg);
				  break;
			default : print_usage();
				  exit(0);
				  break;
		}
			
	}


    if((read_raw_map(infile, &map)) < 0){
        fprintf(stderr, "Failed to read data from file %s\n", infile);
        return EXIT_FAILURE;
    }

    if((write_raw_map2file(outfile, &map)) < 0){
        fprintf(stderr, "Failed to write map to file %s\n", outfile);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static int get_address(char *str) {
    char *endptr;
    long value = strtol(str, &endptr, 0); // 0 autodetects base
    if (*endptr != '\0' || value < 0 || value > 0xFFFF)
        return -1;
    return (int)value;
}