#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "RD_NotificationMessagesRegister.generated.h"


UCLASS()
class RD_MESSAGINGSYSTEM_API URD_NotificationMessagesRegister : public UObject
{
	GENERATED_BODY()

	// State //
	// ===== //
protected:
	TMap<FGameplayTag, UScriptStruct*> RegisteredMessages;


	// Accessors //
	// ========= //
public:
	static const TMap<FGameplayTag, UScriptStruct*>& GetRegisteredMessages();

	
	// Methods //
	// ======= //
public:
	static void RegisterMessage(const FGameplayTag& MessageId, UScriptStruct* PayloadType = nullptr);

	template<typename TPayload>
	static void RegisterMessage(const FGameplayTag& MessageId);
};

template <typename TPayload>
void URD_NotificationMessagesRegister::RegisterMessage(const FGameplayTag& MessageId)
{
	RegisterMessage(MessageId, TBaseStructure<TPayload>::Get());
}