#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Types/RD_RequestMessageRegisterEntry.h"
#include "RD_MessagingSystem/Public/Types/RD_ContextualHandlers.h"
#include "Types/RD_PendingRequests.h"

#include "RD_RequestMessageModule.generated.h"

class IRD_RequestMessageHandlerInterface;

DECLARE_LOG_CATEGORY_EXTERN(LogRDMessagingRequestModule, Log, All);

UCLASS(BlueprintType)
class RD_MESSAGINGSYSTEM_API URD_RequestMessageModule : public UObject
{
	GENERATED_BODY()

	// State //
	// ===== //
private:
	TMap<FGameplayTag, FRD_ContextualHandlers<IRD_RequestMessageHandlerInterface>> RegisteredHandlers;

	UPROPERTY()
	TMap<FGameplayTag, FRD_PendingRequests> PendingRequests;


	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void RegisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		TScriptInterface<IRD_RequestMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	void UnregisterHandler(
		UObject* Context, const FGameplayTag& MessageId,
		TScriptInterface<IRD_RequestMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId"))
	void RegisterGlobalHandler(
		const FGameplayTag& MessageId,
		TScriptInterface<IRD_RequestMessageHandlerInterface> Handler);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId"))
	void UnregisterGlobalHandler(
		const FGameplayTag& MessageId,
		TScriptInterface<IRD_RequestMessageHandlerInterface> Handler);

	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId", DefaultToSelf = "Context"))
	FRD_AsyncMessageResponse SendRequest(
		UObject* Context, const FGameplayTag& MessageId,
		const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	FRD_AsyncMessageResponse SendRequest(
		UObject* Context, const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	FRD_AsyncMessageResponse SendRequest(
		UObject* Context, const FGameplayTag& MessageId, TArgs&&... InArgs);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|RequestModule", meta = (AutoCreateRefTerm = "MessageId"))
	FRD_AsyncMessageResponse SendGlobalRequest(
		const FGameplayTag& MessageId,
		const FInstancedStruct& Payload = FInstancedStruct());
	
	template <typename TPayload>
	FRD_AsyncMessageResponse SendGlobalRequest(
		const FGameplayTag& MessageId, const TPayload& Payload);
	
	template <typename TPayload, typename... TArgs>
	FRD_AsyncMessageResponse SendGlobalRequest(
		const FGameplayTag& MessageId, TArgs&&... InArgs);
	

	// Subroutines //
	// =========== //
private:
	bool ValidateRequest(
		const FGameplayTag& MessageId, const UScriptStruct* PayloadType) const;

	void AddPendingRequest(
		UObject* Context, const FGameplayTag& MessageId,
		const FInstancedStruct& Payload, const FRD_AsyncMessageResponse& AsyncResponse);

	void SendPendingRequests(const FGameplayTag& MessageId);
};

template <typename TPayload>
FRD_AsyncMessageResponse URD_RequestMessageModule::SendRequest(
	UObject* Context, const FGameplayTag& MessageId, const TPayload& Payload)
{
	return SendRequest(Context, MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename ... TArgs>
FRD_AsyncMessageResponse URD_RequestMessageModule::SendRequest(
	UObject* Context, const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	return SendRequest(Context, MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}

template <typename TPayload>
FRD_AsyncMessageResponse URD_RequestMessageModule::SendGlobalRequest(
	const FGameplayTag& MessageId, const TPayload& Payload)
{
	return SendGlobalRequest(MessageId, FInstancedStruct::Make<TPayload>(Payload));
}

template <typename TPayload, typename ... TArgs>
FRD_AsyncMessageResponse URD_RequestMessageModule::SendGlobalRequest(
	const FGameplayTag& MessageId, TArgs&&... InArgs)
{
	return SendGlobalRequest(MessageId, FInstancedStruct::Make<TPayload>(Forward<TArgs>(InArgs)...));
}
