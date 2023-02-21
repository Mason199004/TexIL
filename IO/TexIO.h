//
// Created by Mason on 2/20/2023.
//

#pragma once
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

typedef struct TexIL_TexIO_TexHeader
{
	uint8_t MAGIC[5]; //TexIL in ascii
	uint8_t R1; //
	uint8_t R2; // RESERVED
	uint8_t R3; //
	uint32_t NumLayers;
	float SuggestedAR; //can be 0 for any
	uint64_t PAD1;
	uint64_t PAD2;
	uint8_t NAME[64];
} TexHeader;
_Static_assert(sizeof(TexHeader) == 96, "TexIO.h: TexIL_TexIO_TexHeader is incorrectly sized!");

typedef struct TexIL_TexIO_DataBlock
{
	uint8_t* Data;
	uint32_t Length;
} DataBlock;

typedef struct TexIL_TexIO_TexFile
{
	TexHeader Header;
	DataBlock Data;
} TexFile;

TexFile TexIL_TexIO_ReadFromDisk(char* path);

TexFile TexIL_TexIO_Read(uint8_t* bytes, uint32_t length);

int32_t TexIL_TexIO_DestroyFile(TexFile file);


