#include "RD_MessagingSystem/Public/CommandModule/RD_CommandMessagesRegister.h"

const TMap<FGameplayTag, FRD_CommandMessageRegisterEntry>& URD_CommandMessagesRegister::GetRegisteredMessages()
{
	return GetDefault<ThisClass>()->RegisteredMessages;
}

void URD_CommandMessagesRegister::RegisterMessage(
	const FGameplayTag& MessageId, UScriptStruct* PayloadType,
	const FRD_CommandMessageOptions& Options)
{
	if (!ensure(MessageId.IsValid()))
	{
		return;
	}

	auto* MutableDefault = GetMutableDefault<ThisClass>();
	
	if (MutableDefault->RegisteredMessages.Contains(MessageId))
	{
		ensure(false);
		return;
	}

	MutableDefault->RegisteredMessages.Add(MessageId, { PayloadType, Options });
}
