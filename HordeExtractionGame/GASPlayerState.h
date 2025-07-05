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
class UItemDatabaseComponent;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** The component that manages this player's equipped items. This is now intended to be added as a Blueprint component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	/** The component that stores the player's unique item instances. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UItemDatabaseComponent> ItemDatabaseComponent;

	/** Returns the inventory component to blueprints. */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	/** Returns the equipment component to blueprints. */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	UEquipmentComponent* GetEquipmentComponent() const;

	/** Returns the item database component to blueprints. */
	UFUNCTION(BlueprintPure, Category = "Data")
	UItemDatabaseComponent* GetItemDatabaseComponent() const { return ItemDatabaseComponent; }

protected:
	// The Ability System Component. UPROPERTY() makes it visible to the engine.
		// We add BlueprintReadOnly to allow Blueprints to read this variable.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;
	
	virtual void PostInitializeComponents() override;
	
	// The Attribute Set.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAttributeSet> AttributeSet;
};