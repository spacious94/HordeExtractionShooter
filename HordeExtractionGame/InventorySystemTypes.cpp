//InventorySystemTypes.cpp

#include "InventorySystemTypes.h"
#include "InventoryComponent.h"

// Connect the FastArraySerializer callbacks to the component's Blueprint delegates.

void FItemEntry::PreReplicatedRemove(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast the GUID of the item that is about to be removed.
		InArraySerializer.OwningComponent->OnItemRemoved.Broadcast(this->UniqueID);
	}
}

void FItemEntry::PostReplicatedAdd(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast the data of the newly added item.
		InArraySerializer.OwningComponent->OnItemAdded.Broadcast(*this);
	}
}

void FItemEntry::PostReplicatedChange(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast the new data of the changed item.
		InArraySerializer.OwningComponent->OnItemChanged.Broadcast(*this);
	}
}

bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FItemEntry, FInventoryList>(Items, DeltaParms, *this);
}