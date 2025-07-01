// GASInputID.h
#pragma once
#include "InputAction.h"
#include "GASInputID.generated.h"

// This is our "address book" for gameplay abilities that can be activated by player input.
// It's used to bind abilities to an input ID, which is then bound to a real key in the Editor.
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	Interact			UMETA(DisplayName = "Interact"),
	Sprint			UMETA(DisplayName = "Sprint"),
	Jump			UMETA(DisplayName = "Jump"),
	Crouch			UMETA(DisplayName = "Crouch"),
	ADS			UMETA(DisplayName = "ADS"),
	Reload			UMETA(DisplayName = "Reload"),
	Inventory			UMETA(DisplayName = "Inventory"),
	Fire_Weapon			UMETA(DisplayName = "Fire_Weapon"),
	Melee			UMETA(DisplayName = "Melee"),
	UseTactical			UMETA(DisplayName = "UseTactical"),
	UseLethal			UMETA(DisplayName = "UseLethal"),
	Ability1			UMETA(DisplayName = "Ability1"),
	Ability2			UMETA(DisplayName = "Ability2"),
	Ability3			UMETA(DisplayName = "Ability3"),
	Ability4			UMETA(DisplayName = "Ability4"),
};

USTRUCT(BlueprintType)
struct FInputAbilityBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Binding")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Binding")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
};