// Copyright Your Name or Company Name. All Rights Reserved.

#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Crucial step: Set the back-pointer so the FInventoryList knows which component owns it.
	InventoryList.OwningComponent = this;
}

// Find your GetLifetimeReplicatedProps function and add the new variable to it.
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, InventoryList, COND_OwnerOnly);

}

void UInventoryComponent::AddItem(const FPrimaryAssetId& ItemStaticID, int32 StackSize, int32 Durability)
{
	if (GetOwner()->HasAuthority())
	{
		FItemEntry NewEntry = FItemEntry::Create(ItemStaticID);

		NewEntry.StackSize = StackSize;
		NewEntry.Durability = Durability;

		InventoryList.Items.Add(NewEntry);
		
		// Get a reference to the actual item we just added to the array.
		FItemEntry& AddedEntry = InventoryList.Items.Last();

		// Now use this reference for both MarkItemDirty and the broadcast.
		InventoryList.MarkItemDirty(AddedEntry);

		// The replication callbacks that fire this delegate only execute on clients who receive replicated data.
		// The server/host needs to call it manually since it changes the data directly.
		if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
		{
			OnItemAdded.Broadcast(AddedEntry);
		}
		
	}
}

void UInventoryComponent::Server_MoveItem_Implementation(const FGuid& ItemID, int32 NewX, int32 NewY)
{
	// Loop through all the items in our inventory list.
	// The '&' symbol means we are getting a direct reference to the item, not a copy,
	// so any changes we make will affect the actual item in the array.
	for (FItemEntry& Item : InventoryList.Items)
	{
		// Check if this is the item we are looking for by comparing its UniqueID.
		if (Item.UniqueID == ItemID)
		{
			// TODO: Add collision check to see if the new spot is occupied.
			// For now, we will just move it.

			// Update the item's grid position with the new coordinates.
			Item.GridX = NewX;
			Item.GridY = NewY;

			// This is a crucial step for networking. Because our InventoryList is a "Fast Array Serializer",
			// we must manually tell it which specific item has been changed. This marks it as "dirty"
			// so the server knows to replicate the changes to the client.
			InventoryList.MarkItemDirty(Item);

			// Since we found our item and moved it, we can stop looping.
			break;
		}
	}
}

void UInventoryComponent::Server_RemoveItem_Implementation(const FGuid& ItemID)
{
	// TODO: Add logic to find the item by GUID and remove it from the Items array.
	// TODO: Mark the array dirty.
}

void UInventoryComponent::Server_SplitStack_Implementation(const FGuid& ItemID, int32 AmountToSplit)
{
	// TODO: Add logic to find the original item.
	// TODO: Reduce its stack size.
	// TODO: Create a NEW FItemEntry with a NEW GUID.
	// TODO: Set its stack size to AmountToSplit.
	// TODO: Add the new entry to the Items array.
	// TODO: Mark both items as dirty.
}

void UInventoryComponent::Server_EquipItem_Implementation(const FGuid& ItemID)
{
	// TODO: This will be more complex.
	// TODO: Find the item by GUID.
	// TODO: Remove it from the inventory list.
	// TODO: Add it to a separate "Equipment" list/component.
	// TODO: Grant abilities/effects via the Player State's Ability System Component.
}

TArray<FItemEntry> UInventoryComponent::GetAllItems() const
{
	return InventoryList.Items;
}