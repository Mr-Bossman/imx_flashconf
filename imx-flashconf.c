// SPDX-License-Identifier: (GPL-2.0 or MIT)
/*
 * Copyright (C) 2020 Jesse Taube <Mr.Bossman075@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include "flash-defs.h"
#include "flash_confs.h"

size_t get_name_match(const flash_configs * , const char *);
void help(int);
void prepend(FILE *, size_t);
void list(const flash_configs *);
int main(int argc, char *argv[]){

	char name[32] = {};
	const char * fname;
	int opt;
	size_t index = 0;
	size_t padding = 0x1000; // default offset for nor
	size_t bin = 0;
	FILE * fnum;
	const flash_configs * configs = get_flash_confs();
	const flexspi_nor_config_t * config;
	while ((opt = getopt(argc, argv, "p:n:o:b:lh")) != -1)
	{
		errno = 0;
		switch (opt)
		{
			case 'p':
				padding = strtol(optarg, NULL, 0);
				if (errno) {
				        printf("Invalid value for -%c.\n", opt);
       					help(-3);
				}
				break;
			case 'n':
				strncpy(name,optarg,32);
				break;
			case 'o':
				fname = optarg;
				break;
			case 'b':
				bin = strtol(optarg, NULL, 0);
				if (errno) {
				        printf("Invalid value for -%c.\n", opt);
       					help(-3);
				}
				break;
			case 'h':
				help(0);
				break;
			case 'l':
				list(configs);
				break;
			default:
				help(-3);
				break;
		}
	}

	if(!strnlen(fname,32))
		puts("Need an output file name.");
	if(!strnlen(name,32))
		puts("Need a flash config name.");
	if(strnlen(name,32) == 32)
		puts("Flash config name too long.");
	if(!strnlen(name,32) || !strnlen(fname,32) || strnlen(name,32) == 32)
		help(-3);

	index = get_name_match(configs,name);
	if(index == -1){
		printf("No match found for %s!\nExiting...\n",name);
		exit(-2);
	}
	printf("Match found using %s flash config.\n",configs[index].name);

	config = &configs[index].config;

	fnum = fopen(fname,"wb+");
	if(!fnum){
		printf("Can't open %s for writing!\nExiting...\n",fname);
		exit(-1);
	}

	fwrite(config,sizeof(flexspi_nor_config_t),1,fnum);
	for(size_t i = sizeof(flexspi_nor_config_t); i < padding;i++)
		fputc(0xff,fnum);
	if(bin)
		prepend(fnum,bin);
	fclose(fnum);
	puts("Done!");
	return 0;
}
void list(const flash_configs * configs){
	for(size_t i = 0; *configs[i].name;i++)
		printf("%lu: %s\n",i,configs[i].name);
	exit(0);
}

size_t get_name_match(const flash_configs * configs, const char * name){
	size_t i = 0;
	while(*configs[i].name){
		if(strncmp(configs[i].name,name,32) == 0)
			return i;
		i++;
	}
	return -1;
}
void prepend(FILE * fno, size_t offset){
	FILE * SPL, * uboot;
	char * buf;
	size_t sz;
	uboot = fopen("u-boot.img","r");
	if(!uboot){
		puts("Can't open u-boot.img for reading!\nExiting...");
		fclose(fno);
		exit(-1);
	}
	SPL = fopen("SPL","r");
	if(!SPL){
		puts("Can't open SPL for reading!\nExiting...");
		fclose(uboot);
		fclose(fno);
		exit(-1);
	}
	fseek(SPL, 0, SEEK_END);
	sz = ftell(SPL);
	buf = malloc(sz);
	rewind(SPL);
	fread(buf,sz,1,SPL);
	fwrite(buf,sz,1,fno);

	for(size_t i = ftell(fno); i < offset;i++)
		fputc(0xff,fno);

	fseek(uboot, 0, SEEK_END);
	sz = ftell(uboot);
	buf = realloc(buf, sz);
	rewind(uboot);
	fread(buf,sz,1,uboot);
	fwrite(buf,sz,1,fno);
	free(buf);

	fclose(SPL);
	fclose(uboot);
}

void help(int code){
	puts("+--------------------------------------------------------------+");
	puts("| -h - This message.                                           |");
	puts("| -o - Output file name.                                       |");
	puts("| -n - Name of flash config to use, defined in flash_confs.c . |");
	puts("| -p - Padding offset, other than default (0x1000).            |");
	puts("| -b - Create binary image, and offset to put u-boot.img at.   |");
	puts("| -l - List names of flash defined in flash_confs.c .          |");
	puts("+--------------------------------------------------------------+");
	exit(code);
}