// InventorySystemTypes.cpp

#include "InventorySystemTypes.h"
#include "InventoryComponent.h" // Required for delegate access
#include "HordeExtractionGame.h"

void FItemEntry::PreReplicatedRemove(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->OnItemRemoved.Broadcast(UniqueID);
	}
}

void FItemEntry::PostReplicatedAdd(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->OnItemAdded.Broadcast(*this);
	}
}

void FItemEntry::PostReplicatedChange(const struct FInventoryList& InArraySerializer)
{
	if (InArraySerializer.OwningComponent)
	{
		InArraySerializer.OwningComponent->OnItemChanged.Broadcast(*this);		
	}

	const ENetMode NetMode = InArraySerializer.OwningComponent->GetOwner()
		? InArraySerializer.OwningComponent->GetOwner()->GetNetMode()
		: NM_Standalone;

	const TCHAR* ModeStr = (NetMode == NM_Client) ? TEXT("REMOTE") : TEXT("HOST");

	UE_LOG(LogInventoryUI, Log,
		TEXT("Client %s got item %s at (%d,%d)"),
		ModeStr,
		*UniqueID.ToString(),
		GridX, GridY);
}

bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FItemEntry, FInventoryList>(Items, DeltaParms, *this);
}