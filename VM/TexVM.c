//
// Created by Mason on 2/20/2023.
//
#include <malloc.h>
#include <memory.h>
#include "TexVM.h"
#include <immintrin.h>
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
		#ifdef __AVX512F__ //fast clear layers
		__m512 reg;
		_mm512_xor_si512(reg, reg);
		uint32_t loop = (Height * Width * 4) / 64;
		uint32_t extra = (Height * Width * 4) % 64;
		for (int j = 0; j < loop; ++j) {
			_mm512_storeu_si512(((uint8_t*)vm->Layers[i].RGBAPTR) + (j * 64), reg);
		}
		for (int j = 0; j < extra; ++j) {
			vm->Layers[i].RGBAPTR[loop + j] = 0;
		}
		#else
		__m128 reg;
		_mm_xor_si128(reg, reg);
		uint32_t loop = (Height * Width * 4) / 16;
		uint32_t extra = (Height * Width * 4) % 16;
		for (int j = 0; j < loop; ++j) {
			_mm_storeu_si128(((uint8_t*)vm->Layers[i].RGBAPTR) + (j * 16), reg);
		}
		for (int j = 0; j < extra; ++j) {
			vm->Layers[i].RGBAPTR[loop + j] = 0;
		}
		#endif
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
			case 0x02: {
				uint32_t dest = GetUint(vm->IL, vm->ILIndex + 1);
				uint32_t source = GetUint(vm->IL, vm->ILIndex + 5);
				uint8_t blend = vm->IL[vm->ILIndex + 9];
				switch (blend) {
					case 0: {//add
#ifdef __AVX512F__
						uint32_t loop = (Height * Width * 4) / 64;
						uint32_t extra = (Height * Width * 4) % 64;
						for (int i = 0; i < loop; ++i) {

							__m512i dest_reg = _mm512_loadu_si512(((__m128i *) (vm->Layers[dest].RGBAPTR + (i))));
							__m512i source_reg = _mm512_loadu_si512(((__m128i *) (vm->Layers[source].RGBAPTR + (i))));

							__m512i result = _mm512_adds_epu8(dest_reg, source_reg);

							_mm512_storeu_si512(((__m128i *) (vm->Layers[dest].RGBAPTR + (i))), result);

						}
						for (int i = 0; i < extra / 4; ++i) {
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[0] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[0];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[1] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[1];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[2] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[2];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[3] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[3];
						}
						vm->ILIndex += 9;
						break;
#else
						uint32_t loop = (Height * Width * 4) / 16;
						uint32_t extra = (Height * Width * 4) % 16;
						for (int i = 0; i < loop; ++i) {

							__m128i dest_reg = _mm_loadu_si128(((__m128i *) (vm->Layers[dest].RGBAPTR + (i))));
							__m128i source_reg = _mm_loadu_si128(((__m128i *) (vm->Layers[source].RGBAPTR + (i))));

							__m128i result = _mm_adds_epu8(dest_reg, source_reg);

							_mm_storeu_si128(((__m128i *) (vm->Layers[dest].RGBAPTR + (i))), result);

						}
						for (int i = 0; i < extra / 4; ++i) {
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[0] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[0];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[1] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[1];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[2] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[2];
							((uint8_t *) (vm->Layers[dest].RGBAPTR + (i + loop)))[3] += ((uint8_t *) (
									vm->Layers[source].RGBAPTR + (i + loop)))[3];
						}
						vm->ILIndex += 9;
						break;
#endif
					}
				}
				break;
			}
			case 0x03: //flatten all layers TODO
				break;
			case 0x04: //transform layer TODO
				break;
			case 0x05: {//generate pattern
				uint8_t pattern = vm->IL[vm->ILIndex + 1];
				switch (pattern) {
					case 0: {//grid
						uint8_t flags = vm->IL[vm->ILIndex + 2];

					}
				}
			}

		}
	}
}
#undef GetUint
