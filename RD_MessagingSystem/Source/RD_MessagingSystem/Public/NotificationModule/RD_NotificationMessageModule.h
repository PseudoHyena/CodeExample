#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "RD_MessagingSystem/Public/Types/RD_ContextualHandlers.h"
#include "RD_NotificationMessageModule.generated.h"

class IRD_NotificationMessageHandlerInterface;

DECLARE_LOG_CATEGORY_EXTERN(LogRDMessagingNotificationModule, Log, All);

UCLASS(Within = RD_MessagingSubsystem)
class RD_MESSAGINGSYSTEM_API URD_NotificationMessageModule : public UObject
{
	GENERATED_BODY()
	
	// State //
	// ===== //
private:
	TMap<FGameplayTag, FRD_ContextualHandlers<IRD_NotificationMessageHandlerInterface>> RegisteredHandlers;


	// Accessors //
	// ========= //
public:
	virtual UWorld* GetWorld() const override;

	
	// Lifecycle //
	// ========= //
public:
#if WITH_EDITOR

	virtual bool ImplementsGetWorld() const override { return true; }

#endif
	
	
	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void RegisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler);

	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void UnregisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId"))
	void RegisterGlobalHandler(
		const FGameplayTag& MessageId,
		const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler);

	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId"))
	void UnregisterGlobalHandler(
		const FGameplayTag& MessageId,
		const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler);

	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId, Payload", DefaultToSelf = "Context"))
	void SendNotification(
		UObject* Context, const FGameplayTag& MessageId,
		const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	void SendNotification(UObject* Context, const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	void SendNotification(UObject* Context, const FGameplayTag& MessageId, TArgs&&... InArgs);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|NotificationModule", meta = (AutoCreateRefTerm = "MessageId, Payload"))
	void SendGlobalNotification(
		const FGameplayTag& MessageId, const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	void SendGlobalNotification(const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	void SendGlobalNotification(const FGameplayTag& MessageId, TArgs&&... InArgs);


	// Subroutines //
	// =========== //
private:
	bool ValidateNotification(const FGameplayTag& MessageId, const UScriptStruct* PayloadType);
};

template <typename TPayload>
void URD_NotificationMessageModule::SendNotification(
	UObject* Context, const FGameplayTag& MessageId, const TPayload& Payload)
{
	SendNotification(Context, MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename... TArgs>
void URD_NotificationMessageModule::SendNotification(
	UObject* Context, const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	SendNotification(Context, MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}

template <typename TPayload>
void URD_NotificationMessageModule::SendGlobalNotification(
	const FGameplayTag& MessageId, const TPayload& Payload)
{
	SendGlobalNotification(MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename... TArgs>
void URD_NotificationMessageModule::SendGlobalNotification(
	const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	SendGlobalNotification(MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}
