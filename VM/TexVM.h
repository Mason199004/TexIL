//
// Created by mason on 2023-02-20.
//

#pragma once
#include <stdint.h>
#include "../IO/TexIO.h"

//layer based image generator/manipulator
/*
 * 00 - NOP
 * 01 - SETLAYER, sets current working layer, byte layer
 * 02 - MERGE, merges one layer with another using a blend mode,byte target_layer byte source_layer byte blendmode
 * 03 - FLATTEN, flattens all layers onto layer 0
 * 04 - TRANSFORM, transforms the current layer, vec2 translation float rotation vec2 scale
 * 05 - GEN, generate a pattern, noise, or anything else, byte pattern_type other_args...
 * 06 - DISTORT, distort current layer,byte distortion_type other_args...
 * */

typedef struct TexIL_TexVM_TexImage
{
	uint32_t WIDTH;
	uint32_t HEIGHT;
	uint32_t* RGBAPTR;
} TexImage;

typedef struct TexIL_TexVM_VM
{
	uint32_t ILIndex;
	uint32_t ILSize;
	uint32_t LayerIndex;
	uint32_t NumLayers;
	float SuggestedAR;
	TexImage* Layers;
	uint8_t* IL;
} VM;

VM* TexIL_TexVM_CreateVM(TexFile file);

int32_t TexIL_TexVM_DestroyVM(VM* vm);

TexImage TexIL_TexVM_ExecuteBytecode(VM* vm, uint32_t Width, uint32_t Height);

