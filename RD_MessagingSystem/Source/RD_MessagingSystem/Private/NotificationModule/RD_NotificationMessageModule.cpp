#include "RD_MessagingSystem/Public/NotificationModule/RD_NotificationMessageModule.h"

#include "RD_Generic/Public/Logs/RD_Log.h"
#include "RD_MessagingSystem/Public/NotificationModule/RD_NotificationMessageHandlerInterface.h"
#include "RD_MessagingSystem/Public/NotificationModule/RD_NotificationMessagesRegister.h"
#include "RD_MessagingSystem/Public/Subsystem/RD_MessagingSubsystem.h"

DEFINE_LOG_CATEGORY(LogRDMessagingNotificationModule);

#define LOCTEXT_NAMESPACE "RD_NotificationMessageModule"

UWorld* URD_NotificationMessageModule::GetWorld() const
{
	if (GetOuterURD_MessagingSubsystem())
	{
		return GetOuterURD_MessagingSubsystem()->GetWorld();
	}
	
	return UObject::GetWorld();
}

void URD_NotificationMessageModule::RegisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler)
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

	const TMap<FGameplayTag, UScriptStruct*>& RegisteredMessages =
		URD_NotificationMessagesRegister::GetRegisteredMessages();
	
	if (!RegisteredMessages.Contains(MessageId))
	{
		RD_LOG(LogRDMessagingNotificationModule, Warning, TEXT("Trying to register a Handler for an unregistered message: \"%s\""),
			*MessageId.ToString());
		return;
	}

	auto& Handlers = RegisteredHandlers.FindOrAdd(MessageId);
	Handlers.AddHandler(Context, Handler);
}

void URD_NotificationMessageModule::UnregisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler)
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
	
	const TMap<FGameplayTag, UScriptStruct*>& RegisteredMessages =
		URD_NotificationMessagesRegister::GetRegisteredMessages();
	
	if (!RegisteredMessages.Contains(MessageId))
	{
		RD_LOG(LogRDMessagingNotificationModule, Warning, TEXT("Trying to unregister a Handler for an unregistered message: \"%s\""),
			*MessageId.ToString());
		return;
	}

	if (const auto Handlers = RegisteredHandlers.Find(MessageId))
	{
		Handlers->RemoveHandler(Context, Handler);
	}
}

void URD_NotificationMessageModule::RegisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler)
{
	RegisterHandler(GetWorld(), MessageId, Handler);
}

void URD_NotificationMessageModule::UnregisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_NotificationMessageHandlerInterface>& Handler)
{
	UnregisterHandler(GetWorld(), MessageId, Handler);
}

void URD_NotificationMessageModule::SendNotification(
	UObject* Context, const FGameplayTag& MessageId,
	const FInstancedStruct& Payload)
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}
	
	if (!ensure(ValidateNotification(MessageId, Payload.GetScriptStruct())))
	{
		return;
	}

	if (!RegisteredHandlers.Contains(MessageId))
	{
		return;
	}

	static TArray<TScriptInterface<IRD_NotificationMessageHandlerInterface>> Handlers;
	Handlers.Reset();
	
	RegisteredHandlers[MessageId].GetHandlers(Context, Handlers);
	
	for (TScriptInterface<IRD_NotificationMessageHandlerInterface>& RegisteredHandler : Handlers)
	{
		UObject* HandlerObject = RegisteredHandler.GetObject();
		if (IsValid(HandlerObject))
		{
			IRD_NotificationMessageHandlerInterface::Execute_HandleNotificationMessage(HandlerObject, MessageId, Payload);
		}
	}
}

void URD_NotificationMessageModule::SendGlobalNotification(
	const FGameplayTag& MessageId, const FInstancedStruct& Payload)
{
	SendNotification(GetWorld(), MessageId, Payload);
}

bool URD_NotificationMessageModule::ValidateNotification(
	const FGameplayTag& MessageId, const UScriptStruct* PayloadType)
{
	if (!ensure(MessageId.IsValid()))
	{
		return false;
	}

	const TMap<FGameplayTag, UScriptStruct*>& RegisteredMessages = URD_NotificationMessagesRegister::GetRegisteredMessages();
	
	if (!RegisteredMessages.Contains(MessageId))
	{
		RD_LOG(LogRDMessagingNotificationModule, Warning, TEXT("The message is unregistered: \"%s\""), *MessageId.ToString());
		return false;
	}

	const UScriptStruct* RegisteredPayloadType = RegisteredMessages[MessageId];

	if (RegisteredPayloadType != PayloadType)
	{
		RD_LOG(LogRDMessagingNotificationModule, Warning, TEXT("The message: \"%s\" doesn't expect payload: \"%s\""),
			*MessageId.ToString(), *PayloadType->GetName());
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
