#pragma once

#include "CoreMinimal.h"
#include "RD_CommandMessageOptions.h"

struct FRD_CommandMessageRegisterEntry
{
	// State //
	// ===== //
public:
	UScriptStruct* PayloadType;
	FRD_CommandMessageOptions Options;
};
