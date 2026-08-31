#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RD_MessagingSystemUtils.generated.h"

class URD_MessagingSubsystem;
class URD_RequestMessageModule;
class URD_CommandMessageModule;
class URD_NotificationMessageModule;


UCLASS()
class RD_MESSAGINGSYSTEM_API URD_MessagingSystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	// Accessors //
	// ========= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|Utils", meta = (WorldContext = "WorldContextObject"))
	static URD_NotificationMessageModule* GetNotificationModule(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|Utils", meta = (WorldContext = "WorldContextObject"))
	static URD_CommandMessageModule* GetCommandModule(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|Utils", meta = (WorldContext = "WorldContextObject"))
	static URD_RequestMessageModule* GetRequestModule(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|Utils", meta = (WorldContext = "WorldContextObject"))
	static URD_MessagingSubsystem* GetMessagingSubsystem(const UObject* WorldContextObject);
};
