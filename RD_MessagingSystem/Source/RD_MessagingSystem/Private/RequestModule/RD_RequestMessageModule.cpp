#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessageModule.h"

#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessageHandlerInterface.h"
#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessagesRegister.h"
#include "RD_Generic/Public/Logs/RD_Log.h"

DEFINE_LOG_CATEGORY(LogRDMessagingRequestModule);

void URD_RequestMessageModule::RegisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	TScriptInterface<IRD_RequestMessageHandlerInterface> Handler)
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
		TScriptInterface<IRD_RequestMessageHandlerInterface> ExistingHandler;
		const auto bHandlerAlreadyExists = RegisteredHandlers[MessageId].GetFirstHandler(Context, ExistingHandler);

		if (bHandlerAlreadyExists)
		{
			RD_LOG(LogRDMessagingRequestModule, Warning, TEXT("Trying to register another handler: \"%s\" for same message twise: \"%s\""),
				*Handler.GetObject()->GetClass()->GetName(), *MessageId.ToString());
			
			return;
		}
	}

	RegisteredHandlers.FindOrAdd(MessageId).AddHandler(Context, Handler);
	
	SendPendingRequests(MessageId);
}

void URD_RequestMessageModule::UnregisterHandler(
	UObject* Context, const FGameplayTag& MessageId,
	const TScriptInterface<IRD_RequestMessageHandlerInterface> Handler)
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

void URD_RequestMessageModule::RegisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_RequestMessageHandlerInterface> Handler)
{
	RegisterHandler(GetWorld(), MessageId, Handler);
}

void URD_RequestMessageModule::UnregisterGlobalHandler(
	const FGameplayTag& MessageId,
	const TScriptInterface<IRD_RequestMessageHandlerInterface> Handler)
{
	UnregisterHandler(GetWorld(), MessageId, Handler);
}

FRD_AsyncMessageResponse URD_RequestMessageModule::SendRequest(
	UObject* Context, const FGameplayTag& MessageId,
	const FInstancedStruct& Payload)
{
	if (!ensure(IsValid(Context)))
	{
		return {};
	}
	
	if (!ensure(ValidateRequest(MessageId, Payload.GetScriptStruct())))
	{
		return {};
	}

	FRD_AsyncMessageResponse AsyncResponse;

	if (PendingRequests.Contains(MessageId) &&
		PendingRequests[MessageId].Requests.Contains(Context))
	{
		const FRD_PendingRequest& PendingRequest = PendingRequests[MessageId].Requests[Context];

		AsyncResponse = PendingRequest.AsyncResponse;
	}
	else
	{
		AsyncResponse = FRD_AsyncMessageResponse::Make(MessageId);
	}
	
	if (!RegisteredHandlers.Contains(MessageId))
	{
		AddPendingRequest(Context, MessageId, Payload, AsyncResponse);
		return AsyncResponse;
	}

	TScriptInterface<IRD_RequestMessageHandlerInterface> Handler;
	RegisteredHandlers[MessageId].GetFirstHandler(Context, Handler);
	
	if (!IsValid(Handler.GetObject()))
	{
		AddPendingRequest(Context, MessageId, Payload, AsyncResponse);
		return AsyncResponse;
	}

	IRD_RequestMessageHandlerInterface::Execute_HandleRequestMessage(
		Handler.GetObject(), MessageId, Payload, AsyncResponse);

	return AsyncResponse;
}

FRD_AsyncMessageResponse URD_RequestMessageModule::SendGlobalRequest(
	const FGameplayTag& MessageId,
	const FInstancedStruct& Payload)
{
	return SendRequest(GetWorld(), MessageId, Payload);
}

bool URD_RequestMessageModule::ValidateRequest(
	const FGameplayTag& MessageId, const UScriptStruct* PayloadType) const
{
	if (!ensure(MessageId.IsValid()))
	{
		return false;
	}

	const TMap<FGameplayTag, FRD_RequestMessageRegisterEntry>& RegisteredMessages =
		URD_RequestMessagesRegister::GetRegisteredMessages();

	if (!RegisteredMessages.Contains(MessageId))
	{
		RD_LOG(LogRDMessagingRequestModule, Warning, TEXT("Trying to send an unregistered message: \"%s\""), *MessageId.ToString());
		return false;
	}

	const FRD_RequestMessageRegisterEntry& RegisteredMessageEntry = RegisteredMessages[MessageId];

	if (PayloadType != RegisteredMessageEntry.PayloadType)
	{
		RD_LOG(LogRDMessagingRequestModule, Warning, TEXT("Trying to send a message: \"%s\" with unexpected payload: \"%s\""),
			*MessageId.ToString(), *PayloadType->GetName());
		return false;
	}

	return true;
}

void URD_RequestMessageModule::AddPendingRequest(
	UObject* Context, const FGameplayTag& MessageId,
	const FInstancedStruct& Payload, const FRD_AsyncMessageResponse& AsyncResponse)
{
	const TMap<FGameplayTag, FRD_RequestMessageRegisterEntry>& RegisteredMessages =
		URD_RequestMessagesRegister::GetRegisteredMessages();

	if (!ensure(RegisteredMessages.Contains(MessageId)))
	{
		return;
	}
	
	const FRD_RequestMessageRegisterEntry& RegisteredMessageEntry =
		RegisteredMessages[MessageId];

	auto& [Requests] = PendingRequests.FindOrAdd(MessageId);

	switch (RegisteredMessageEntry.Options.StackRule)
	{
	case ERD_RequestMessageStackRule::Stack:
		Requests.Add(Context, { MessageId, Payload, AsyncResponse });
		
		break;
		
	case ERD_RequestMessageStackRule::Override:
		Requests.Remove(Context);
		Requests.Add(Context, { MessageId, Payload, AsyncResponse });
		
		break;
		
	case ERD_RequestMessageStackRule::Ignore:
		if (!Requests.Contains(Context))
		{
			Requests.Add(Context, { MessageId, Payload, AsyncResponse });
		}

		break;
	}
}

void URD_RequestMessageModule::SendPendingRequests(
	const FGameplayTag& MessageId)
{
	if (!PendingRequests.Contains(MessageId))
	{
		return;
	}

	TMap<TObjectKey<UObject>, FRD_PendingRequest>& Requests = PendingRequests[MessageId].Requests;
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		UObject* Context = It->Key.ResolveObjectPtr();
		if (!IsValid(Context))
		{
			continue;
		}

		FRD_AsyncMessageResponse AsyncMessageResponse = SendRequest(Context, MessageId, It->Value.Payload);
		if (AsyncMessageResponse.IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
