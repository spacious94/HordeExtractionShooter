// InventoryViewModel.cpp

#include "InventoryViewModel.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.h"
#include "ItemDatabaseComponent.h"
#include "GASPlayerState.h"
#include "HordeExtractionGame.h"

// --- Private Helper Function ---
UItemDatabaseComponent* UInventoryViewModel::GetDatabase() const
{
	if (InventoryComponent)
	{
		if (AGASPlayerState* PS = InventoryComponent->GetOwner<AGASPlayerState>())
		{
			return PS->GetItemDatabaseComponent();
		}
	}
	return nullptr;
}


void UInventoryViewModel::Initialize(UInventoryComponent* InInventoryComponent, UEquipmentComponent* InEquipmentComponent)
{
	if (bIsInitialized)
	{
		return;
	}

	if (InInventoryComponent)
	{
		InventoryComponent = InInventoryComponent;
		InventoryComponent->OnItemAdded.AddDynamic(this, &UInventoryViewModel::HandleInventoryChanged);
		InventoryComponent->OnItemChanged.AddDynamic(this, &UInventoryViewModel::HandleInventoryChanged);
		InventoryComponent->OnItemRemoved.AddDynamic(this, &UInventoryViewModel::HandleItemRemoved);
	}

	if (InEquipmentComponent)
	{
		EquipmentComponent = InEquipmentComponent;
		EquipmentComponent->OnEquipmentChanged.AddDynamic(this, &UInventoryViewModel::HandleEquipmentChanged);
	}

	bIsInitialized = true;
	UE_LOG(LogInventoryUI, Log, TEXT("ViewModel Initialized and bound to Inventory and Equipment Components."));
}

TArray<FItemEntry> UInventoryViewModel::GetInventoryItems() const
{
	if (InventoryComponent)
	{
		TArray<FItemEntry> AllItems = InventoryComponent->GetAllItems();
		return AllItems.FilterByPredicate([](const FItemEntry& Item)
			{
				return !Item.bPendingRemoval;
			});
	}
	return TArray<FItemEntry>();
}

void UInventoryViewModel::MoveInventoryItem(const FGuid& ItemID, int32 NewX, int32 NewY)
{
	if (InventoryComponent)
	{
		InventoryComponent->Server_MoveItem(ItemID, NewX, NewY);
	}
}

bool UInventoryViewModel::IsInventorySpaceAvailable(FIntPoint TopLeft, FIntPoint Size, const FGuid& IgnoreItemID) const
{
	if (InventoryComponent)
	{
		return InventoryComponent->IsSpaceAvailable(TopLeft, Size, IgnoreItemID);
	}
	return false;
}

void UInventoryViewModel::RequestDropItem(const FGuid& ItemID, EEquipmentSlot SourceSlot)
{
	if (SourceSlot != EEquipmentSlot::None)
	{
		if (EquipmentComponent)
		{
			EquipmentComponent->DropEquippedItem(SourceSlot);
		}
	}
	else
	{
		if (InventoryComponent)
		{
			InventoryComponent->Server_DropItem(ItemID);
		}
	}
}

// --- NEW: Implementation for our helper function ---
bool UInventoryViewModel::GetItemInstanceByID(const FGuid& ItemID, FItemInstance& OutItem) const
{
	if (UItemDatabaseComponent* DB = GetDatabase())
	{
		if (const FItemInstance* ItemPtr = DB->GetClientItemInstance(ItemID))
		{
			OutItem = *ItemPtr;
			return true;
		}
	}
	return false;
}

void UInventoryViewModel::RequestEquipItem(EEquipmentSlot Slot, const FGuid& ItemID)
{
	UE_LOG(LogInventoryUI, Log, TEXT("UInventoryViewModel::RequestEquipItem - Received request."));
	if (EquipmentComponent)
	{
		UE_LOG(LogInventoryUI, Log, TEXT("UInventoryViewModel::RequestEquipItem - EquipmentComponent is valid, calling EquipItem."));
		EquipmentComponent->EquipItem(Slot, ItemID);
	}
}

void UInventoryViewModel::RequestUnequipItem(EEquipmentSlot Slot)
{
	if (EquipmentComponent)
	{
		EquipmentComponent->UnequipItem(Slot);
	}
}

bool UInventoryViewModel::GetEquippedItemForSlot(EEquipmentSlot Slot, FItemInstance& OutItem) const
{
	if (EquipmentComponent)
	{
		return EquipmentComponent->GetEquippedItem(Slot, OutItem);
	}
	return false;
}

void UInventoryViewModel::HandleInventoryChanged(const FItemEntry& ItemEntry)
{
	OnInventoryChanged.Broadcast();
}

void UInventoryViewModel::HandleItemRemoved(const FGuid& ItemID)
{
	OnInventoryChanged.Broadcast();
}

void UInventoryViewModel::HandleEquipmentChanged(EEquipmentSlot Slot, const FItemInstance& ItemInstance)
{
	OnEquipmentChanged.Broadcast(Slot, ItemInstance);
}