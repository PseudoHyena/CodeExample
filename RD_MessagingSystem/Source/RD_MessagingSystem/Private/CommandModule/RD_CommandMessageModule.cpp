#include "RD_MessagingSystem/Public/CommandModule/RD_CommandMessageModule.h"

#include "RD_MessagingSystem/Public/CommandModule/RD_CommandMessageHandlerInterface.h"
#include "RD_MessagingSystem/Public/CommandModule/RD_CommandMessagesRegister.h"
#include "RD_Generic/Public/Logs/RD_Log.h"

DEFINE_LOG_CATEGORY(LogRDMessagingCommandModule);

void URD_CommandMessageModule::RegisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	TScriptInterface<IRD_CommandMessageHandlerInterface> Handler)
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}
	
	if (!ensure(MessageId.IsValid()))
	{
		return;
	}
	
	if (!ensure(IsValid(Handler.GetObject())))
	{
		return;
	}
	
	if (RegisteredHandlers.Contains(MessageId))
	{
		TScriptInterface<IRD_CommandMessageHandlerInterface> ExistingHandler;
		const auto bHandlerAlreadyExists = RegisteredHandlers[MessageId].GetFirstHandler(Context, ExistingHandler);

		if (bHandlerAlreadyExists)
		{
			RD_LOG(LogRDMessagingCommandModule, Warning, TEXT("Trying to register another handler: \"%s\" for same message twise: \"%s\""),
				*Handler.GetObject()->GetClass()->GetName(), *MessageId.ToString());
			
			return;
		}
	}

	RegisteredHandlers.FindOrAdd(MessageId).AddHandler(Context, Handler);
	
	SendPendingCommands(MessageId);
}

void URD_CommandMessageModule::UnregisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	const TScriptInterface<IRD_CommandMessageHandlerInterface> Handler)
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}
	
	if (!ensure(MessageId.IsValid()))
	{
		return;
	}
	
	if (!ensure(IsValid(Handler.GetObject())))
	{
		return;
	}

	if (RegisteredHandlers.Contains(MessageId))
	{
		RegisteredHandlers[MessageId].RemoveHandler(Context, Handler);
	}
}

void URD_CommandMessageModule::RegisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_CommandMessageHandlerInterface> Handler)
{
	RegisterHandler(GetWorld(), MessageId, Handler);
}

void URD_CommandMessageModule::UnregisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_CommandMessageHandlerInterface> Handler)
{
	UnregisterHandler(GetWorld(), MessageId, Handler);
}

bool URD_CommandMessageModule::SendCommand(
	UObject* Context, const FGameplayTag& MessageId,
	const FInstancedStruct& Payload)
{
	if (!ensure(IsValid(Context)))
	{
		return false;
	}
	
	if (!ensure(ValidateCommand(MessageId, Payload.GetScriptStruct())))
	{
		return false;
	}

	if (!RegisteredHandlers.Contains(MessageId))
	{
		return false;
	}

	TScriptInterface<IRD_CommandMessageHandlerInterface> Handler;
	RegisteredHandlers[MessageId].GetFirstHandler(Context, Handler);
	
	if (IsValid(Handler.GetObject()))
	{
		IRD_CommandMessageHandlerInterface::Execute_HandleCommandMessage(
			Handler.GetObject(), MessageId, Payload);

		return true;
	}

	AddPendingCommand(Context, MessageId, Payload);
	return false;
}

bool URD_CommandMessageModule::SendGlobalCommand(
	const FGameplayTag& MessageId, const FInstancedStruct& Payload)
{
	return SendCommand(GetWorld(), MessageId, Payload);
}

bool URD_CommandMessageModule::ValidateCommand(
	const FGameplayTag& MessageId, const UScriptStruct* PayloadType) const
{
	if (!ensure(MessageId.IsValid()))
	{
		return false;
	}
	
	const TMap<FGameplayTag, FRD_CommandMessageRegisterEntry>& RegisteredMessages =
		URD_CommandMessagesRegister::GetRegisteredMessages();

	if (!RegisteredMessages.Contains(MessageId))
	{
		RD_LOG(LogRDMessagingCommandModule, Warning, TEXT("Trying to send an unregistered message: \"%s\""), *MessageId.ToString());
		return false;
	}

	const FRD_CommandMessageRegisterEntry& RegisteredMessageEntry = RegisteredMessages[MessageId];

	if (PayloadType != RegisteredMessageEntry.PayloadType)
	{
		RD_LOG(LogRDMessagingCommandModule, Warning, TEXT("Trying to send a message: \"%s\" with unexpected payload: \"%s\""),
			*MessageId.ToString(), *PayloadType->GetName());
		return false;
	}

	return true;
}

void URD_CommandMessageModule::AddPendingCommand(
	UObject* Context, const FGameplayTag& MessageId,
	const FInstancedStruct& Payload)
{
	const TMap<FGameplayTag, FRD_CommandMessageRegisterEntry>& RegisteredMessages =
		URD_CommandMessagesRegister::GetRegisteredMessages();

	if (!ensure(RegisteredMessages.Contains(MessageId)))
	{
		return;
	}
	
	const FRD_CommandMessageRegisterEntry& RegisteredMessageEntry =
		RegisteredMessages[MessageId];

	auto& [Commands] = PendingCommands.FindOrAdd(MessageId);

	switch (RegisteredMessageEntry.Options.StackRule)
	{
	case ERD_CommandMessageStackRule::Stack:
		Commands.Add(Context, { MessageId, Payload });
		
		break;
		
	case ERD_CommandMessageStackRule::Override:
		Commands.Remove(Context);
		Commands.Add(Context, { MessageId, Payload });
		
		break;
		
	case ERD_CommandMessageStackRule::Ignore:
		if (!Commands.Contains(Context))
		{
			Commands.Add(Context, { MessageId, Payload });
		}

		break;
	}
}

void URD_CommandMessageModule::SendPendingCommands(
	const FGameplayTag& MessageId)
{
	if (!PendingCommands.Contains(MessageId))
	{
		return;
	}

	TMap<TObjectKey<UObject>, FRD_PendingCommand>& Commands = PendingCommands[MessageId].Commands;
	for (auto It = Commands.CreateIterator(); It; ++It)
	{
		UObject* Context = It->Key.ResolveObjectPtr();
		if (!IsValid(Context))
		{
			continue;
		}

		if (SendCommand(Context, MessageId, It->Value.Payload))
		{
			It.RemoveCurrent();
		}
	}
}
