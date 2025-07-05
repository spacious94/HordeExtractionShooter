// ItemDatabaseComponent.cpp
#include "ItemDatabaseComponent.h"
#include "Net/UnrealNetwork.h"

#include "GASPlayerState.h"
#include "InventoryComponent.h"

#include "HordeExtractionGame.h" // For LogInventoryUI

// --- FReplicatedItemInstance Implementation ---

void FReplicatedItemInstance::PreReplicatedRemove(const FItemInstanceList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->RebuildClientDatabase();
	}
}

void FReplicatedItemInstance::PostReplicatedAdd(const FItemInstanceList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->RebuildClientDatabase();
	}
}

void FReplicatedItemInstance::PostReplicatedChange(const FItemInstanceList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->RebuildClientDatabase();
	}
}

// --- FItemInstanceList Implementation ---

bool FItemInstanceList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FReplicatedItemInstance, FItemInstanceList>(Items, DeltaParms, *this);
}

// --- UItemDatabaseComponent Implementation ---

UItemDatabaseComponent::UItemDatabaseComponent()
{
	SetIsReplicatedByDefault(true);
}

void UItemDatabaseComponent::Server_CreateNewItemInstance(const FItemInstance& ItemInstance)
{
	if (!GetOwner()->HasAuthority()) return;

	UE_LOG(LogInventoryUI, Log, TEXT("Database::Server_CreateNewItemInstance - Received request to create item with ID: %s"), *ItemInstance.InstanceID.ToString());

	// 1. Add the heavy instance to the database
	AddItem(ItemInstance);

	// 2. Add the lightweight entry to the inventory
	AGASPlayerState* PlayerState = GetOwner<AGASPlayerState>();
	if (PlayerState)
	{
		if (UInventoryComponent* InventoryComp = PlayerState->GetInventoryComponent())
		{
			// We need to create a new FItemEntry and ensure it uses the SAME GUID as the FItemInstance.
			FItemEntry NewEntry;
			NewEntry.UniqueID = ItemInstance.InstanceID; // This is the crucial link.
			NewEntry.StaticDataID = ItemInstance.StaticDataID;
			
			// Assuming stack size and durability are stored on the instance or are defaulted.
			InventoryComp->AddItemFromEntry(NewEntry, 1, 100);
		}
	}
}

void UItemDatabaseComponent::BeginPlay()
{
	Super::BeginPlay();
	ReplicatedItems.OwningComponent = this;
}

void UItemDatabaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UItemDatabaseComponent, ReplicatedItems, COND_OwnerOnly);
}

void UItemDatabaseComponent::OnRep_ReplicatedItems()
{
	// This function is called on the client when the ReplicatedItems array changes.
	// We simply call our helper function to rebuild the local TMap.
	RebuildClientDatabase();
}

void UItemDatabaseComponent::RebuildClientDatabase()
{
	ClientDatabase.Empty();
	for (const FReplicatedItemInstance& ReplicatedInstance : ReplicatedItems.Items)
	{
		ClientDatabase.Add(ReplicatedInstance.Item.InstanceID, ReplicatedInstance.Item);
	}
}

const FItemInstance* UItemDatabaseComponent::GetClientItemInstance(const FGuid& ItemID) const
{
	// On the client, we look in our local, non-authoritative database.
	return ClientDatabase.Find(ItemID);
}

// --- Server-Only Functions ---

void UItemDatabaseComponent::AddItem(const FItemInstance& ItemInstance)
{
	if (!GetOwner()->HasAuthority()) return;

	ServerDatabase.Add(ItemInstance.InstanceID, ItemInstance);

	FReplicatedItemInstance NewReplicatedInstance;
	NewReplicatedInstance.Item = ItemInstance;
	ReplicatedItems.Items.Add(NewReplicatedInstance);
	ReplicatedItems.MarkItemDirty(ReplicatedItems.Items.Last());
}

void UItemDatabaseComponent::RemoveItem(const FGuid& ItemID)
{
	if (!GetOwner()->HasAuthority()) return;

	ServerDatabase.Remove(ItemID);

	ReplicatedItems.Items.RemoveAll([&ItemID](const FReplicatedItemInstance& Instance) {
		return Instance.Item.InstanceID == ItemID;
	});
	ReplicatedItems.MarkArrayDirty();
}

const FItemInstance* UItemDatabaseComponent::GetItemInstance(const FGuid& ItemID) const
{
	// On the server, we look in the authoritative database.
	return ServerDatabase.Find(ItemID);
}
