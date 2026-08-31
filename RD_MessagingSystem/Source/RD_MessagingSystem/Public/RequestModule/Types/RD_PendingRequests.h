#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "RD_AsyncMessageResponse.h"
#include "RD_PendingRequests.generated.h"


USTRUCT()
struct RD_MESSAGINGSYSTEM_API FRD_PendingRequest
{
	GENERATED_BODY()
	
	// State //
	// ===== //
public:
	FGameplayTag MessageId;
	FInstancedStruct Payload;
	FRD_AsyncMessageResponse AsyncResponse;
};


USTRUCT()
struct RD_MESSAGINGSYSTEM_API FRD_PendingRequests
{
	GENERATED_BODY()
	
	// State //
	// ===== //
public:
	TMap<TObjectKey<UObject>, FRD_PendingRequest> Requests;
};
