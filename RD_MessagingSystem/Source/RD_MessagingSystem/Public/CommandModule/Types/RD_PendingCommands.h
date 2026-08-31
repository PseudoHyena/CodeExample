#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "RD_PendingCommands.generated.h"


USTRUCT()
struct RD_MESSAGINGSYSTEM_API FRD_PendingCommand
{
	GENERATED_BODY()
	
	// State //
	// ===== //
public:
	FGameplayTag MessageId;
	FInstancedStruct Payload;
};


USTRUCT()
struct RD_MESSAGINGSYSTEM_API FRD_PendingCommands
{
	GENERATED_BODY()
	
	// State //
	// ===== //
public:
	TMap<TObjectKey<UObject>, FRD_PendingCommand> Commands;
};
