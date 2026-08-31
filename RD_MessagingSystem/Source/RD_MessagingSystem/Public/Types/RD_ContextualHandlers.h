#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"


template<typename THandlerInterface>
struct FRD_ContextualHandlers
{
	struct FRD_ContextualHandler;
	
	// State //
	// ===== //
private:
	TMap<int32, FRD_ContextualHandler> Handlers;
	
	TMap<TObjectKey<UObject>, TArray<int32>> PerContextHandlerIds;
	
	int32 IdCounter;


	// Accessors //
	// ========= //
public:
	void GetHandlers(UObject* Context, TArray<TScriptInterface<THandlerInterface>>& OutHandlers) const;
	bool GetFirstHandler(UObject* Context, TScriptInterface<THandlerInterface>& OutHandler) const;


	// Lifecycle //
	// ========= //
public:
	FRD_ContextualHandlers();
	

	// Methods //
	// ======= //
public:
	void AddHandler(UObject* Context, const TScriptInterface<THandlerInterface>& Handler);
	void RemoveHandler(UObject* Context, const TScriptInterface<THandlerInterface>& Handler);


	// Subtypes //
	// ======== //
private:
	struct FRD_ContextualHandler
	{
		// State //
		// ===== //
	public:
		TObjectKey<UObject> Context;
		TScriptInterface<THandlerInterface> Handler;
	};
};

template <typename THandlerInterface>
void FRD_ContextualHandlers<THandlerInterface>::GetHandlers(
	UObject* Context, TArray<TScriptInterface<THandlerInterface>>& OutHandlers) const
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}

	UObject* ContextIteration = Context;
	while (ContextIteration)
	{
		if (PerContextHandlerIds.Contains(ContextIteration))
		{
			for (const int32 Id : PerContextHandlerIds[ContextIteration])
			{
				if (Handlers.Contains(Id))
				{
					OutHandlers.Add(Handlers[Id].Handler);
				}
			}
		}

		if (ContextIteration->IsA<UWorld>())
		{
			break;
		}
		
		ContextIteration = ContextIteration->GetOuter();
	}
}

template <typename THandlerInterface>
bool FRD_ContextualHandlers<THandlerInterface>::GetFirstHandler(
	UObject* Context, TScriptInterface<THandlerInterface>& OutHandler) const
{
	if (!ensure(IsValid(Context)))
	{
		return false;
	}

	UObject* ContextIteration = Context;
	while (ContextIteration)
	{
		if (PerContextHandlerIds.Contains(ContextIteration))
		{
			for (const int32 Id : PerContextHandlerIds[ContextIteration])
			{
				if (Handlers.Contains(Id))
				{
					OutHandler = Handlers[Id].Handler;
					return true;
				}
			}
		}

		if (ContextIteration->IsA<UWorld>())
		{
			break;
		}
		
		ContextIteration = ContextIteration->GetOuter();
	}

	return false;
}

template <typename THandlerInterface>
FRD_ContextualHandlers<THandlerInterface>::FRD_ContextualHandlers()
	: IdCounter(INDEX_NONE)
{
}

template <typename THandlerInterface>
void FRD_ContextualHandlers<THandlerInterface>::AddHandler(
	UObject* Context, const TScriptInterface<THandlerInterface>& Handler)
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}
	
	if (!Context->IsA<UWorld>() && !IsValid(Context->GetTypedOuter<UWorld>()))
	{
		return;
	}

	const int32 Id = ++IdCounter;
	Handlers.Add(Id, { Context, Handler });
	PerContextHandlerIds.FindOrAdd(Context).Add(Id);
}

template <typename THandlerInterface>
void FRD_ContextualHandlers<THandlerInterface>::RemoveHandler(
	UObject* Context, const TScriptInterface<THandlerInterface>& Handler)
{
	if (!ensure(IsValid(Context)))
	{
		return;
	}

	int32 Id = {};
	for (const auto& HandlerPair : Handlers)
	{
		const FRD_ContextualHandler& ContextualHandler = HandlerPair.Value;
		if (ContextualHandler.Context == Context && ContextualHandler.Handler == Handler)
		{
			Id = HandlerPair.Key;
			break;
		}
	}

	if (Id == INDEX_NONE)
	{
		return;
	}

	if (PerContextHandlerIds.Contains(Context))
	{
		PerContextHandlerIds[Context].Remove(Id);
	}

	Handlers.Remove(Id);
}
