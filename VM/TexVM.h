//
// Created by mason on 2023-02-20.
//

#pragma once
#include <stdint.h>
#include "../IO/TexIO.h"

//layer based image generator/manipulator
/*
 * 00 - NOP
 * 01 - SETLAYER, sets current working layer, uint32 layer
 * 02 - MERGE, merges one layer with another using a blend mode,uint32 target_layer uint32 source_layer byte blendmode
 * 03 - FLATTEN, flattens all layers onto layer 0
 * 04 - TRANSFORM, transforms the current layer, vec2 translation float rotation vec2 scale
 * 05 - GEN, generate a pattern, noise, or anything else, byte pattern_type other_args...
 * 06 - DISTORT, distort current layer,byte distortion_type other_args...
 * */


/**
 * Raw image data
 */
typedef struct TexIL_TexVM_TexImage
{
	uint32_t WIDTH; ///Width of the image
	uint32_t HEIGHT; ///Height of the image
	uint32_t* RGBAPTR; ///Pointer to raw pixel data in RGBA format
} TexImage;
/**
 * Object containing IL bytecode as well as other important information to be used to generate image
 */
typedef struct TexIL_TexVM_VM
{
	uint32_t ILIndex; ///Current index into the IL
	uint32_t ILSize; ///Total size of all IL bytes
	uint32_t LayerIndex; ///Current index into Layers
	uint32_t NumLayers; ///Number of Layers
	float SuggestedAR; ///Suggested aspect ratio of image to be produced
	TexImage* Layers; ///Pointer to Layer Array
	uint8_t* IL; ///Pointer to IL bytecode
} VM;

VM* TexIL_TexVM_CreateVM(TexFile file);

int32_t TexIL_TexVM_DestroyVM(VM* vm);

TexImage TexIL_TexVM_ExecuteBytecode(VM* vm, uint32_t Width, uint32_t Height);

