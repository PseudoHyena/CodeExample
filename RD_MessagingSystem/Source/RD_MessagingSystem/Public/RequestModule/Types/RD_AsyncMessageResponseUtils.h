#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "RD_AsyncMessageResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RD_AsyncMessageResponseUtils.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FRD_AsyncMessageResponseDelegate, FInstancedStruct, Value);

UCLASS()
class RD_MESSAGINGSYSTEM_API URD_AsyncMessageResponseUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Accessors //
	// ========= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|AsyncMessageResponse")
	static bool TryGetValue(UPARAM(ref) FRD_AsyncMessageResponse& Response, FInstancedStruct& OutValue);
	

	// Methods //
	// ======= //
public:
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|AsyncMessageResponse")
	static bool Respond(UPARAM(ref) FRD_AsyncMessageResponse& Response, FInstancedStruct Value);
	
	UFUNCTION(BlueprintCallable, Category = "RD|MessagingSystem|AsyncMessageResponse")
	static void Subscribe(
		UPARAM(ref) FRD_AsyncMessageResponse& Response, UObject* ContextObject, FRD_AsyncMessageResponseDelegate Event);
};
