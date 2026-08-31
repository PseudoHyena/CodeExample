#pragma once

#include "CoreMinimal.h"
#include "RD_RequestMessageStackRule.h"
#include "RD_RequestMessageOptions.generated.h"


USTRUCT(BlueprintType)
struct FRD_RequestMessageOptions
{
	GENERATED_BODY()

	// State //
	// ===== //
public:
	UPROPERTY(BlueprintReadWrite)
	ERD_RequestMessageStackRule StackRule;


	// Lifecycle //
	// ========= //
public:
	FRD_RequestMessageOptions()
		: StackRule(ERD_RequestMessageStackRule::Stack)
	{ }

	FRD_RequestMessageOptions(const ERD_RequestMessageStackRule InStackRule)
		: StackRule(InStackRule)
	{ }
};
