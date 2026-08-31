#pragma once

#include "CoreMinimal.h"
#include "RD_RequestMessageStackRule.generated.h"


UENUM(BlueprintType)
enum class ERD_RequestMessageStackRule : uint8
{
	Stack,
	Override,
	Ignore
};
