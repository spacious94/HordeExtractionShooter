//GASPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h" // Required for GAS
#include "GASPlayerState.generated.h"

// Forward declare our classes to avoid circular dependencies
class UGASAbilitySystemComponent;
class UGASAttributeSet;
class UInventoryComponent;

UCLASS()
class HORDEEXTRACTIONGAME_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASPlayerState();

	// This is the function from IAbilitySystemInterface that the engine calls to get our ASC.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// A helper function to get our custom attribute set
	UGASAttributeSet* GetAttributeSet() const;

	/** The component that manages this player's inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Returns the inventory component to blueprints. */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	// The Ability System Component. UPROPERTY() makes it visible to the engine.
		// We add BlueprintReadOnly to allow Blueprints to read this variable.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;

	// The Attribute Set.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAttributeSet> AttributeSet;
};