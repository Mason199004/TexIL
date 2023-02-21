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

	vm->ILSize = file.Data.Length;
	vm->IL = malloc(vm->ILSize);
	memcpy(vm->IL, file.Data.Data, file.Data.Length);

	vm->SuggestedAR = file.Header.SuggestedAR;

	return vm;
}

TexImage TexIL_TexVM_ExecuteBytecode(VM* vm, uint32_t Width, uint32_t Height)
{

}
