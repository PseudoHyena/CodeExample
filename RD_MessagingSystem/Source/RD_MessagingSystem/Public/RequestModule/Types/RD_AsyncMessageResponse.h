#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RD_Generic/Public/Async/RD_AsyncValue.h"
#include "StructUtils/InstancedStruct.h"
#include "RD_AsyncMessageResponse.generated.h"


USTRUCT(BlueprintType)
struct RD_MESSAGINGSYSTEM_API FRD_AsyncMessageResponse
{
	GENERATED_BODY()
	
	// State //
	// ===== //
private:
	UPROPERTY()
	UScriptStruct* ResponseType;
	
	TSharedRef<TRD_AsyncValue<FInstancedStruct>> AsyncResponse;

	
	// Accessors //
	// ========= //
public:
	bool TryGetValue(FInstancedStruct& OutValue) const;
	

	// Lifecycle //
	// ========= //
public:
	FRD_AsyncMessageResponse();
	explicit FRD_AsyncMessageResponse(UScriptStruct* Type);


	// Methods //
	// ======= //
public:
	static FRD_AsyncMessageResponse Make(const FGameplayTag& MessageId);

	bool Respond(const FInstancedStruct& Value);
	
	template<typename T>
	bool Respond();

	template<typename T>
	bool Respond(const T& Struct);

	template<typename T, typename... TArgs>
	bool Respond(TArgs&&... InArgs);

	template<typename FunctorType, typename... VarTypes>
	FDelegateHandle BindLambda(
		FunctorType&& InFunctor, VarTypes... Vars);

	template<typename UserClass, typename FunctorType, typename... VarTypes>
	FDelegateHandle BindWeakLambda(
		UserClass* InUserObject, FunctorType&& InFunctor, VarTypes... Vars);

	template<typename UserClass, typename... VarTypes>
	FDelegateHandle BindUObject(
		UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(const FInstancedStruct&, VarTypes...)>::Type InFunc, VarTypes... Vars);

	template<typename UserClass, typename... VarTypes>
	FDelegateHandle BindUObject(
		const UserClass* InUserObject,
		typename TMemFunPtrType<true, UserClass, void(const FInstancedStruct&, VarTypes...)>::Type InFunc, VarTypes... Vars);

	bool IsValid() const;
	bool HasResponded() const;
	
};

template <typename T>
bool FRD_AsyncMessageResponse::Respond()
{
	if (!!IsValid() || TBaseStructure<T>::Get() != ResponseType)
	{
		return false;
	}

	AsyncResponse->Emplace(FInstancedStruct::Make<T>());
	return true;
}

template <typename T>
bool FRD_AsyncMessageResponse::Respond(const T& Struct)
{
	if (!IsValid() || TBaseStructure<T>::Get() != ResponseType)
	{
		return false;
	}

	AsyncResponse->Emplace(FInstancedStruct::Make<T>(Struct));
	return true;
}

template <typename T, typename ... TArgs>
bool FRD_AsyncMessageResponse::Respond(TArgs&&... InArgs)
{
	if (!IsValid() || TBaseStructure<T>::Get() != ResponseType)
	{
		return false;
	}

	AsyncResponse->Emplace(FInstancedStruct::Make<T>(Forward<TArgs>(InArgs)...));
	return true;
}

template <typename FunctorType, typename ... VarTypes>
FDelegateHandle FRD_AsyncMessageResponse::BindLambda(
	FunctorType&& InFunctor, VarTypes... Vars)
{
	return AsyncResponse->BindLambda(InFunctor, Vars...);
}

template <typename UserClass, typename FunctorType, typename ... VarTypes>
FDelegateHandle FRD_AsyncMessageResponse::BindWeakLambda(
	UserClass* InUserObject, FunctorType&& InFunctor, VarTypes... Vars)
{
	return AsyncResponse->BindWeakLambda(InUserObject, InFunctor, Vars...);
}

template <typename UserClass, typename ... VarTypes>
FDelegateHandle FRD_AsyncMessageResponse::BindUObject(
	UserClass* InUserObject,
	typename TMemFunPtrType<false, UserClass, void(const FInstancedStruct&, VarTypes...)>::Type InFunc, VarTypes... Vars)
{
	return AsyncResponse->BindUObject(InUserObject, InFunc, Vars...);
}

template <typename UserClass, typename ... VarTypes>
FDelegateHandle FRD_AsyncMessageResponse::BindUObject(
	const UserClass* InUserObject,
	typename TMemFunPtrType<true, UserClass, void(const FInstancedStruct&, VarTypes...)>::Type InFunc, VarTypes... Vars)
{
	return AsyncResponse->BindUObject(InUserObject, InFunc, Vars...);
}
