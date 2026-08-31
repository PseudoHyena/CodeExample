#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RD_MessagingSystem/Public/Types/RD_ContextualHandlers.h"
#include "StructUtils/InstancedStruct.h"
#include "Types/RD_PendingCommands.h"

#include "RD_CommandMessageModule.generated.h"

class IRD_CommandMessageHandlerInterface;

DECLARE_LOG_CATEGORY_EXTERN(LogRDMessagingCommandModule, Log, All);

UCLASS(BlueprintType)
class RD_MESSAGINGSYSTEM_API URD_CommandMessageModule : public UObject
{
	GENERATED_BODY()

	// State //
	// ===== //
private:
	TMap<FGameplayTag, FRD_ContextualHandlers<IRD_CommandMessageHandlerInterface>> RegisteredHandlers;

	UPROPERTY()
	TMap<FGameplayTag, FRD_PendingCommands> PendingCommands;


	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void RegisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		TScriptInterface<IRD_CommandMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void UnregisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		TScriptInterface<IRD_CommandMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId"))
	void RegisterGlobalHandler(
		const FGameplayTag& MessageId,
		TScriptInterface<IRD_CommandMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId"))
	void UnregisterGlobalHandler(
		const FGameplayTag& MessageId,
		TScriptInterface<IRD_CommandMessageHandlerInterface> Handler);

	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	bool SendCommand(
		UObject* Context, const FGameplayTag& MessageId,
		const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	bool SendCommand(
		UObject* Context,const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	bool SendCommand(
		UObject* Context,const FGameplayTag& MessageId, TArgs&&... InArgs);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|CommandModule", meta = (AutoCreateRefTerm = "MessageId"))
	bool SendGlobalCommand(const FGameplayTag& MessageId, const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	bool SendGlobalCommand(const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	bool SendGlobalCommand(const FGameplayTag& MessageId, TArgs&&... InArgs);
	

	// Subroutines //
	// =========== //
private:
	bool ValidateCommand(
		const FGameplayTag& MessageId, const UScriptStruct* PayloadType) const;

	void AddPendingCommand(
		UObject* Context, const FGameplayTag& MessageId,
		const FInstancedStruct& Payload);

	void SendPendingCommands(const FGameplayTag& MessageId);
};

template <typename TPayload>
bool URD_CommandMessageModule::SendCommand(
	UObject* Context, const FGameplayTag& MessageId, const TPayload& Payload)
{
	return SendCommand(Context, MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename... TArgs>
bool URD_CommandMessageModule::SendCommand(
	UObject* Context, const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	return SendCommand(Context, MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}

template <typename TPayload>
bool URD_CommandMessageModule::SendGlobalCommand(
	const FGameplayTag& MessageId, const TPayload& Payload)
{
	return SendGlobalCommand(MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename ... TArgs>
bool URD_CommandMessageModule::SendGlobalCommand(
	const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	return SendGlobalCommand(MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}
