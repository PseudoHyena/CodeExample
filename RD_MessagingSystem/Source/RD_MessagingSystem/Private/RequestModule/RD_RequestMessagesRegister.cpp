#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessagesRegister.h"

const TMap<FGameplayTag, FRD_RequestMessageRegisterEntry>& URD_RequestMessagesRegister::GetRegisteredMessages()
{
	return GetDefault<ThisClass>()->RegisteredMessages;
}

void URD_RequestMessagesRegister::RegisterMessage(
	const FGameplayTag& MessageId,
	UScriptStruct* ResponseType, UScriptStruct* PayloadType,
	const FRD_RequestMessageOptions& Options)
{
	if (!ensure(MessageId.IsValid()))
	{
		return;
	}

	if (!ensure(IsValid(ResponseType)))
	{
		return;
	}

	auto* MutableDefault = GetMutableDefault<ThisClass>();
	
	if (MutableDefault->RegisteredMessages.Contains(MessageId))
	{
		ensure(false);
		return;
	}

	MutableDefault->RegisteredMessages.Add(
		MessageId, { PayloadType, ResponseType, Options });
}
