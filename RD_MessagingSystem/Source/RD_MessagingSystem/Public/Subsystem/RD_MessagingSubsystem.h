#pragma once

#include "CoreMinimal.h"
#include "RD_Generic/Public/Macros/RD_PropertyAccessorsMacros.h"
#include "RD_MessagingSubsystem.generated.h"

class URD_CommandMessageModule;
class URD_NotificationMessageModule;
class URD_RequestMessageModule;


UCLASS()
class RD_MESSAGINGSYSTEM_API URD_MessagingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	// Subobjects //
	// ========== //
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URD_RequestMessageModule> RequestMessageModule;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URD_NotificationMessageModule> NotificationMessageModule;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URD_CommandMessageModule> CommandMessageModule;
	
	
	// Accessors //
	// ========= //
public:
	GETTER_COPY(URD_RequestMessageModule*, RequestMessageModule);
	GETTER_COPY(URD_NotificationMessageModule*, NotificationMessageModule);
	GETTER_COPY(URD_CommandMessageModule*, CommandMessageModule);


	// Lifecycle //
	// ========= //
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
