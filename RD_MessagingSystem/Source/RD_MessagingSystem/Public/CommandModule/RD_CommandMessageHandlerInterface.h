#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Interface.h"
#include "RD_CommandMessageHandlerInterface.generated.h"


UINTERFACE()
class RD_MESSAGINGSYSTEM_API URD_CommandMessageHandlerInterface : public UInterface
{
	GENERATED_BODY()
};


class RD_MESSAGINGSYSTEM_API IRD_CommandMessageHandlerInterface
{
	GENERATED_BODY()

	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintNativeEvent, Category = "RD|MessagingSystem|CommandMessageHandlerInterface")
	void HandleCommandMessage(const FGameplayTag& MessageId, const FInstancedStruct& Payload);
};
