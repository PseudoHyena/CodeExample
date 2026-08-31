#include "RD_MessagingSystem/Public/Subsystem/RD_MessagingSubsystem.h"

#include "RD_MessagingSystem/Public/CommandModule/RD_CommandMessageModule.h"
#include "RD_MessagingSystem/Public/NotificationModule/RD_NotificationMessageModule.h"
#include "RD_MessagingSystem/Public/RequestModule/RD_RequestMessageModule.h"

void URD_MessagingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RequestMessageModule = NewObject<URD_RequestMessageModule>(this);
	NotificationMessageModule = NewObject<URD_NotificationMessageModule>(this);
	CommandMessageModule = NewObject<URD_CommandMessageModule>(this); 
}

void URD_MessagingSubsystem::Deinitialize()
{
	Super::Deinitialize();

	RequestMessageModule = nullptr;
	NotificationMessageModule = nullptr;
	CommandMessageModule = nullptr; 
}
