//
// Created by Mason on 2/20/2023.
//
#include <malloc.h>
#include <memory.h>
#include "TexVM.h"
#include "../IO/TexIO.h"

VM* TexIL_TexVM_CreateVM(TexFile file)
{
	VM* vm = (VM*)malloc(sizeof(VM));
	memset(vm, 0, sizeof(VM)); //0 it out just in case

	vm->NumLayers = file.Header.NumLayers;
	vm->Layers = malloc(sizeof(TexImage) * vm->NumLayers);
	for (int i = 0; i < vm->NumLayers; ++i)
	{
		vm->Layers[i].RGBAPTR = NULL;
	}

	vm->ILSize = file.Data.Length;
	vm->IL = malloc(vm->ILSize);
	memcpy(vm->IL, file.Data.Data, file.Data.Length);

	vm->SuggestedAR = file.Header.SuggestedAR;

	return vm;
}

uint32_t TexIL_TexVM_UIntFromPTR(const uint8_t* data, uint32_t index)
{
	return ((((uint32_t)data[index + 1]) << 24) & (((uint32_t)data[index + 2]) << 16) & (((uint32_t)data[index + 3]) << 8) & (((uint32_t)data[index + 4])));
}
#define GetUint TexIL_TexVM_UIntFromPTR

TexImage TexIL_TexVM_ExecuteBytecode(VM* vm, uint32_t Width, uint32_t Height)
{
	TexImage image;
	image.WIDTH = Width;
	image.HEIGHT = Height;
	image.RGBAPTR = malloc(Width * Height * sizeof(uint32_t));

	for (int i = 0; i < vm->NumLayers; ++i)
	{
		vm->Layers[i].WIDTH = Width;
		vm->Layers[i].HEIGHT = Height;
		vm->Layers[i].RGBAPTR = malloc(Width * Height * sizeof(uint32_t));
	}

	uint32_t CurrentLayer = 0;
	while (vm->ILIndex < vm->ILSize)
	{
		switch (vm->IL[vm->ILIndex]) {
			case 0x00:
				//nop
				vm->ILIndex++;
				break;
			case 0x01:
				CurrentLayer = GetUint(vm->IL, vm->ILIndex + 1);
				vm->ILIndex += 5;
				break;
			case 0x02:
				uint32_t dest = GetUint(vm->IL, vm->ILIndex + 1);
				uint32_t source = GetUint(vm->IL, vm->ILIndex + 5);
				uint8_t blend = vm->IL[vm->ILIndex + 9];
				switch (blend) {
					case 0: //add
						for (int i = 0; i < vm->Layers[source].HEIGHT; ++i) {
							for (int j = 0; j < vm->Layers[source].WIDTH; ++j) {
								((uint8_t*)(vm->Layers[dest].RGBAPTR + (i * Height + j)))[0] += ((uint8_t*)(vm->Layers[source].RGBAPTR + (i * Height + j)))[0];
								((uint8_t*)(vm->Layers[dest].RGBAPTR + (i * Height + j)))[1] += ((uint8_t*)(vm->Layers[source].RGBAPTR + (i * Height + j)))[1];
								((uint8_t*)(vm->Layers[dest].RGBAPTR + (i * Height + j)))[2] += ((uint8_t*)(vm->Layers[source].RGBAPTR + (i * Height + j)))[2];
								((uint8_t*)(vm->Layers[dest].RGBAPTR + (i * Height + j)))[3] += ((uint8_t*)(vm->Layers[source].RGBAPTR + (i * Height + j)))[3];
							}
						}
						vm->ILIndex += 9;
						break;
				}
				break;
			case 0x03: //flatten all layers TODO
				break;
			case 0x04: //transform layer TODO
				break;
			case 0x05: //generate pattern
				uint8_t pattern = vm->IL[vm->ILIndex + 1];


		}
	}
}
#undef GetUint
