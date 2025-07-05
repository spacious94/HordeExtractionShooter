// AbilityInputBinding.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GASInputID.h" // For EAbilityInputID
#include "AbilityInputBinding.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAbilityInputBinding : public UInterface
{
	GENERATED_BODY()
};

class HORDEEXTRACTIONGAME_API IAbilityInputBinding
{
	GENERATED_BODY()

public:
	// This function must be implemented by any Gameplay Ability that wants to be bound to an input.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	EAbilityInputID GetAbilityInputID();
};
