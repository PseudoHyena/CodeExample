#include "RD_MessagingSystem/Public/RequestModule/Types/RD_AsyncMessageResponseUtils.h"

bool URD_AsyncMessageResponseUtils::TryGetValue(
	FRD_AsyncMessageResponse& Response, FInstancedStruct& OutValue)
{
	return Response.TryGetValue(OutValue);
}

bool URD_AsyncMessageResponseUtils::Respond(
	FRD_AsyncMessageResponse& Response, const FInstancedStruct Value)
{
	return Response.Respond(Value);
}

void URD_AsyncMessageResponseUtils::Subscribe(
	FRD_AsyncMessageResponse& Response, UObject* ContextObject, FRD_AsyncMessageResponseDelegate Event)
{
	if (!ensure(IsValid(ContextObject)))
	{
		return;
	}
	
	Response.BindWeakLambda(ContextObject, [Event](const FInstancedStruct& Value)
	{
		Event.ExecuteIfBound(Value);
	});
}
