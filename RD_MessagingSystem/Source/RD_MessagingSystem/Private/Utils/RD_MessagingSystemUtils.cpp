#include "RD_MessagingSystem/Public/Utils/RD_MessagingSystemUtils.h"

#include "RD_MessagingSystem/Public/Subsystem/RD_MessagingSubsystem.h"

URD_NotificationMessageModule* URD_MessagingSystemUtils::GetNotificationModule(
	const UObject* WorldContextObject)
{
	URD_MessagingSubsystem* MessagingSubsystem = GetMessagingSubsystem(WorldContextObject);
	if (!ensure(IsValid(MessagingSubsystem)))
	{
		return nullptr;
	}
	
	return MessagingSubsystem->GetNotificationMessageModule();
}

URD_CommandMessageModule* URD_MessagingSystemUtils::GetCommandModule(
	const UObject* WorldContextObject)
{
	URD_MessagingSubsystem* MessagingSubsystem = GetMessagingSubsystem(WorldContextObject);
	if (!ensure(IsValid(MessagingSubsystem)))
	{
		return nullptr;
	}
	
	return MessagingSubsystem->GetCommandMessageModule();
}

URD_RequestMessageModule* URD_MessagingSystemUtils::GetRequestModule(
	const UObject* WorldContextObject)
{
	URD_MessagingSubsystem* MessagingSubsystem = GetMessagingSubsystem(WorldContextObject);
	if (!ensure(IsValid(MessagingSubsystem)))
	{
		return nullptr;
	}
	
	return MessagingSubsystem->GetRequestMessageModule();
}

URD_MessagingSubsystem* URD_MessagingSystemUtils::GetMessagingSubsystem(
	const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!ensure(World))
	{
		return nullptr;
	}
	
	return World->GetSubsystem<URD_MessagingSubsystem>();
}
