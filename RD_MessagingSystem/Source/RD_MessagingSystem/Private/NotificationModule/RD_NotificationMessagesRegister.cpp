#include "RD_MessagingSystem/Public/NotificationModule/RD_NotificationMessagesRegister.h"

const TMap<FGameplayTag, UScriptStruct*>& URD_NotificationMessagesRegister::GetRegisteredMessages()
{
	return GetDefault<ThisClass>()->RegisteredMessages;
}

void URD_NotificationMessagesRegister::RegisterMessage(
	const FGameplayTag& MessageId, UScriptStruct* PayloadType)
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

	MutableDefault->RegisteredMessages.Add(MessageId, PayloadType);
}
