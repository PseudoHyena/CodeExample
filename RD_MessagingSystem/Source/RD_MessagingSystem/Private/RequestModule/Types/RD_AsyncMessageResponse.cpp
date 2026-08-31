#include "RD_MessagingSystem/Public/RequestModule/Types/RD_AsyncMessageResponse.h"

#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessagesRegister.h"
#include "RD_MessagingSystem/Public/RequestModule/Types/RD_RequestMessageRegisterEntry.h"

bool FRD_AsyncMessageResponse::TryGetValue(FInstancedStruct& OutValue) const
{
	if (HasResponded())
	{
		OutValue = AsyncResponse->GetValue();
		return true;
	}

	return false;
}

FRD_AsyncMessageResponse::FRD_AsyncMessageResponse()
	: FRD_AsyncMessageResponse(nullptr)
{
}

FRD_AsyncMessageResponse::FRD_AsyncMessageResponse(UScriptStruct* Type)
	: ResponseType(Type)
	, AsyncResponse(MakeShared<TRD_AsyncValue<FInstancedStruct>>())
{
}

FRD_AsyncMessageResponse FRD_AsyncMessageResponse::Make(
	const FGameplayTag& MessageId)
{
	const TMap<FGameplayTag, FRD_RequestMessageRegisterEntry>& RegisteredMessages =
		URD_RequestMessagesRegister::GetRegisteredMessages();

	if (!RegisteredMessages.Contains(MessageId))
	{
		return {};
	}

	const FRD_RequestMessageRegisterEntry& RegisteredMessageEntry = RegisteredMessages[MessageId];
	return FRD_AsyncMessageResponse(RegisteredMessageEntry.ResponseType);
}

bool FRD_AsyncMessageResponse::Respond(const FInstancedStruct& Value)
{
	if (!IsValid() || Value.GetScriptStruct() != ResponseType)
	{
		return false;
	}

	AsyncResponse->Emplace(Value);
	return true;
}

bool FRD_AsyncMessageResponse::IsValid() const
{
	return ::IsValid(ResponseType);
}

bool FRD_AsyncMessageResponse::HasResponded() const
{
	return IsValid() && AsyncResponse->IsSet();
}
