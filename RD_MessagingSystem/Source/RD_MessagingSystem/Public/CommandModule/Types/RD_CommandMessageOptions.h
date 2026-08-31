#pragma once

#include "CoreMinimal.h"
#include "RD_CommandMessageStackRule.h"
#include "RD_CommandMessageOptions.generated.h"


USTRUCT(BlueprintType)
struct FRD_CommandMessageOptions
{
	GENERATED_BODY()

	// State //
	// ===== //
public:
	UPROPERTY(BlueprintReadWrite)
	ERD_CommandMessageStackRule StackRule;


	// Lifecycle //
	// ========= //
public:
	FRD_CommandMessageOptions()
		: StackRule(ERD_CommandMessageStackRule::Stack)
	{ }

	FRD_CommandMessageOptions(const ERD_CommandMessageStackRule InStackRule)
		: StackRule(InStackRule)
	{ }
};
