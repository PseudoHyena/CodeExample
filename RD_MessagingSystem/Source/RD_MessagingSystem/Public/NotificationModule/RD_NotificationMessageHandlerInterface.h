#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Interface.h"
#include "RD_NotificationMessageHandlerInterface.generated.h"


UINTERFACE()
class RD_MESSAGINGSYSTEM_API URD_NotificationMessageHandlerInterface : public UInterface
{
	GENERATED_BODY()
};


class RD_MESSAGINGSYSTEM_API IRD_NotificationMessageHandlerInterface
{
	GENERATED_BODY()

	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintNativeEvent, Category = "RD|MessagingSystem|NotificationMessageHandlerInterface")
	void HandleNotificationMessage(const FGameplayTag& MessageId, const FInstancedStruct& Payload);
};
