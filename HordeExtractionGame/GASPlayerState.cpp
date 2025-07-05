// GASPlayerState.cpp

#include "GASPlayerState.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.h" // Include the new component

#include "ItemDatabaseComponent.h"

AGASPlayerState::AGASPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	// Set the replication mode to Mixed. This is the standard for player-controlled characters.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the Attribute Set
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");

	// Set the update frequency on the PlayerState.
	NetUpdateFrequency = 100.0f;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);

	ItemDatabaseComponent = CreateDefaultSubobject<UItemDatabaseComponent>(TEXT("ItemDatabaseComponent"));
	ItemDatabaseComponent->SetIsReplicated(true);

	// EquipmentComponent is now added in Blueprint, not here.
}

void AGASPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Find the component that was added in Blueprint
	if (EquipmentComponent == nullptr)
	{
		EquipmentComponent = FindComponentByClass<UEquipmentComponent>();
	}
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGASAttributeSet* AGASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

UEquipmentComponent* AGASPlayerState::GetEquipmentComponent() const
{
	return EquipmentComponent;
}