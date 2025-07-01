// GASPlayerState.cpp

#include "GASPlayerState.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "InventoryComponent.h"

AGASPlayerState::AGASPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // <-- CRITICAL FIX 1: This is required for multiplayer.

	// Set the replication mode to Mixed. This is the standard for player-controlled characters.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // <-- CRITICAL FIX 2: Correct replication mode.

	// Create the Attribute Set
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");

	// Set the update frequency on the PlayerState.
	// This is a best practice for GAS to ensure attributes and tags replicate quickly.
	NetUpdateFrequency = 100.0f; // <-- PERFORMANCE FIX: From the documentation.

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGASAttributeSet* AGASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}