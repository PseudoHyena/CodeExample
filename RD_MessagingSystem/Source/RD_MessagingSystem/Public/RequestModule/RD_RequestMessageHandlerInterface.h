#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Types/RD_AsyncMessageResponse.h"
#include "UObject/Interface.h"
#include "RD_RequestMessageHandlerInterface.generated.h"


UINTERFACE()
class RD_MESSAGINGSYSTEM_API URD_RequestMessageHandlerInterface : public UInterface
{
	GENERATED_BODY()
};


class RD_MESSAGINGSYSTEM_API IRD_RequestMessageHandlerInterface
{
	GENERATED_BODY()

	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintNativeEvent, Category = "RD|MessagingSystem|RequestMessageHandlerInterface")
	void HandleRequestMessage(
		const FGameplayTag& MessageId,
		const FInstancedStruct& Payload, FRD_AsyncMessageResponse Response);
};
