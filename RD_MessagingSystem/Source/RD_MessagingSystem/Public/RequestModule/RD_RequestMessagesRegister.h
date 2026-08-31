#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/RD_RequestMessageRegisterEntry.h"

#include "RD_RequestMessagesRegister.generated.h"


UCLASS()
class RD_MESSAGINGSYSTEM_API URD_RequestMessagesRegister : public UObject
{
	GENERATED_BODY()

	// State //
	// ===== //
protected:
	TMap<FGameplayTag, FRD_RequestMessageRegisterEntry> RegisteredMessages;


	// Accessors //
	// ========= //
public:
	static const TMap<FGameplayTag, FRD_RequestMessageRegisterEntry>& GetRegisteredMessages();

	
	// Methods //
	// ======= //
public:
	static void RegisterMessage(
		const FGameplayTag& MessageId,
		UScriptStruct* ResponseType, UScriptStruct* PayloadType,
		const FRD_RequestMessageOptions& Options);

	template<typename TResponse, typename TPayload>
	static void RegisterMessage(
		const FGameplayTag& MessageId,
		const FRD_RequestMessageOptions& Options);

	template<typename TResponse>
	static void RegisterMessage(
		const FGameplayTag& MessageId,
		const FRD_RequestMessageOptions& Options);
};

template <typename TResponse, typename TPayload>
void URD_RequestMessagesRegister::RegisterMessage(
	const FGameplayTag& MessageId,
	const FRD_RequestMessageOptions& Options)
{
	RegisterMessage(MessageId, TBaseStructure<TResponse>::Get(), TBaseStructure<TPayload>::Get(), Options);
}

template <typename TResponse>
void URD_RequestMessagesRegister::RegisterMessage(
	const FGameplayTag& MessageId, const FRD_RequestMessageOptions& Options)
{
	RegisterMessage(MessageId, TBaseStructure<TResponse>::Get(), nullptr, Options);
}
