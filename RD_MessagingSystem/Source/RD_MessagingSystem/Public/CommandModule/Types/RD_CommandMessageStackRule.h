#pragma once

#include "CoreMinimal.h"
#include "RD_CommandMessageStackRule.generated.h"


UENUM(BlueprintType)
enum class ERD_CommandMessageStackRule : uint8
{
	Stack,
	Override,
	Ignore
};
