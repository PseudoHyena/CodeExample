#pragma once

#include "CoreMinimal.h"
#include "RD_RequestMessageOptions.h"

struct FRD_RequestMessageRegisterEntry
{
	// State //
	// ===== //
public:
	UScriptStruct* PayloadType;
	UScriptStruct* ResponseType;
	FRD_RequestMessageOptions Options;
};
