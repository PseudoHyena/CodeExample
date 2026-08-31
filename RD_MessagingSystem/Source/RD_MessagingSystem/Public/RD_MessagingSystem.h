#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRD_MessagingSystemModule : public IModuleInterface
{
	// Lifecycle //
	// ========= //
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
