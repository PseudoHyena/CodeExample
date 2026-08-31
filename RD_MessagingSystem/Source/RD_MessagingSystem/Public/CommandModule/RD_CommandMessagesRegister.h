#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/RD_CommandMessageRegisterEntry.h"

#include "RD_CommandMessagesRegister.generated.h"


UCLASS()
class RD_MESSAGINGSYSTEM_API URD_CommandMessagesRegister : public UObject
{
	GENERATED_BODY()

	// State //
	// ===== //
protected:
	TMap<FGameplayTag, FRD_CommandMessageRegisterEntry> RegisteredMessages;


	// Accessors //
	// ========= //
public:
	static const TMap<FGameplayTag, FRD_CommandMessageRegisterEntry>& GetRegisteredMessages();

	
	// Methods //
	// ======= //
public:
	static void RegisterMessage(
		const FGameplayTag& MessageId, UScriptStruct* PayloadType = nullptr, const FRD_CommandMessageOptions& Options = {});

	template<typename TPayload>
	static void RegisterMessage(const FGameplayTag& MessageId, const FRD_CommandMessageOptions& Options = {});
};

template <typename TPayload>
void URD_CommandMessagesRegister::RegisterMessage(const FGameplayTag& MessageId, const FRD_CommandMessageOptions& Options)
{
	RegisterMessage(MessageId, TBaseStructure<TPayload>::Get(), Options);
}