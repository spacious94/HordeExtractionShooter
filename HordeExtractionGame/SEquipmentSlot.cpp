// SEquipmentSlot.cpp
#include "SEquipmentSlot.h"
#include "BrushCache.h"
#include "DragDropOps.h"
#include "InventoryViewModel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"

#include "HordeExtractionGame.h" // For LogInventoryUI

void SEquipmentSlot::Construct(const FArguments& InArgs)
{
	ViewModel = InArgs._ViewModel;
	SlotType = InArgs._SlotType;
	TileSize = InArgs._TileSize;

	if (ViewModel)
	{
		ViewModel->OnEquipmentChanged.AddSP(this, &SEquipmentSlot::RefreshSlot);
	}

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(TileSize)
		.HeightOverride(TileSize)
		[
			SAssignNew(ItemIcon, SImage)
		]
	];

	// Initial update
	FItemInstance InitialItem;
	if (ViewModel)
	{
		ViewModel->GetEquippedItemForSlot(SlotType, InitialItem);
	}
	RefreshSlot(SlotType, InitialItem);
}

FReply SEquipmentSlot::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FInventoryDragDropOp> Op = DragDropEvent.GetOperationAs<FInventoryDragDropOp>();
	if (Op.IsValid())
	{
		UE_LOG(LogInventoryUI, Log, TEXT("SEquipmentSlot::OnDrop - SUCCESS, DragOp is valid."));
		if (ViewModel)
		{
			UE_LOG(LogInventoryUI, Log, TEXT("SEquipmentSlot::OnDrop - ViewModel is valid, calling RequestEquipItem."));
			ViewModel->RequestEquipItem(SlotType, Op->ItemID);
		}
		return FReply::Handled();
	}
	UE_LOG(LogInventoryUI, Warning, TEXT("SEquipmentSlot::OnDrop - FAILED, DragOp was not FInventoryDragDropOp."));
	return FReply::Unhandled();
}

void SEquipmentSlot::SetViewModel(UInventoryViewModel* InViewModel)
{
	if (ViewModel)
	{
		ViewModel->OnEquipmentChanged.RemoveAll(this);
	}

	ViewModel = InViewModel;

	if (ViewModel)
	{
		ViewModel->OnEquipmentChanged.AddSP(this, &SEquipmentSlot::RefreshSlot);
	}
}

void SEquipmentSlot::RefreshSlot(EEquipmentSlot Slot, const FItemInstance& Item)
{
	// Only update if the change is for this specific slot
	if (Slot != SlotType)
	{
		return;
	}

	if (Item.InstanceID.IsValid())
	{
		if (UBrushCache* Cache = UBrushCache::Get())
		{
			ItemIcon->SetImage(Cache->GetBrush(Item.StaticDataID).Get());
			ItemIcon->SetVisibility(EVisibility::Visible);
			return;
		}
	}

	// If we get here, the slot is empty or the item data is invalid.
	ItemIcon->SetImage(nullptr);
	ItemIcon->SetVisibility(EVisibility::Hidden);
}

