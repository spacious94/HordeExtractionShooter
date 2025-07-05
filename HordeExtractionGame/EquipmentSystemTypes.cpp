// EquipmentSystemTypes.cpp
#include "EquipmentSystemTypes.h"
#include "EquipmentComponent.h"

void FEquippedItem::PreReplicatedRemove(const FEquippedList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast with an empty item instance to signify removal for the UI
		InArraySerializer.OwningComponent->OnEquipmentChanged.Broadcast(Slot, FItemInstance());
	}
}

void FEquippedItem::PostReplicatedAdd(const FEquippedList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast the change to the UI
		InArraySerializer.OwningComponent->OnEquipmentChanged.Broadcast(Slot, Item);
	}
}

void FEquippedItem::PostReplicatedChange(const FEquippedList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		// Broadcast the change to the UI
		InArraySerializer.OwningComponent->OnEquipmentChanged.Broadcast(Slot, Item);
	}
}

bool FEquippedList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FEquippedItem, FEquippedList>(Items, DeltaParms, *this);
}
