// SEquipmentSlot.cpp
#include "SEquipmentSlot.h"
#include "BrushCache.h"
#include "DragDropOps.h"
#include "InventoryViewModel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "HordeFunctionLibrary.h"
#include "Engine/AssetManager.h"   // <-- ADDED for validation
#include "BaseItemDataAsset.h"     // <-- ADDED for validation

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
	if (Op.IsValid() && ViewModel)
	{
		bool bPassesValidation = false;

		// --- C++ VALIDATION LOGIC ---
		if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			// Get the item's static data from the Asset Manager
			if (UBaseItemDataAsset* ItemData = Cast<UBaseItemDataAsset>(AssetManager->GetPrimaryAssetObject(Op->StaticDataID)))
			{
				// If the ValidSlots array is empty, we treat it as a wildcard (allowed in any slot).
				// Otherwise, we check if the array contains the slot type of this widget.
				if (ItemData->ValidSlots.Num() == 0 || ItemData->ValidSlots.Contains(SlotType))
				{
					bPassesValidation = true;
				}
			}
		}

		if (bPassesValidation)
		{
			// Validation passed, request the equip.
			ViewModel->RequestEquipItem(SlotType, Op->ItemID);
			Op->bDropSucceeded = true;
		}
		// If validation fails, we do nothing. The drop is still "handled" to prevent
		// a world drop, and the item will snap back because bDropSucceeded is false.

		return FReply::Handled();
	}

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

FReply SEquipmentSlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FItemInstance ItemInSlot;
		if (ViewModel && ViewModel->GetEquippedItemForSlot(SlotType, ItemInSlot))
		{
			return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
		}
	}
	return FReply::Unhandled();
}

FReply SEquipmentSlot::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FItemInstance ItemInSlot;
	if (!ViewModel || !ViewModel->GetEquippedItemForSlot(SlotType, ItemInSlot))
	{
		return FReply::Unhandled();
	}

	const FSlateBrush* Brush = nullptr;
	if (UBrushCache* Cache = UBrushCache::Get())
	{
		Brush = Cache->GetBrush(ItemInSlot.StaticDataID).Get();
	}

	const FIntPoint ItemSize = UHordeFunctionLibrary::GetItemSize(ItemInSlot.StaticDataID);
	const FVector2D WidgetSize = FVector2D(ItemSize.X * TileSize, ItemSize.Y * TileSize);

	TSharedRef<SWidget> Decorator = SNew(SBox)
		.WidthOverride(WidgetSize.X)
		.HeightOverride(WidgetSize.Y)
		[
			SNew(SImage)
				.Image(Brush)
				.ColorAndOpacity(FLinearColor(1, 1, 1, 0.75f))
		];

	const FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const FVector2D NewOffset = -LocalMousePosition;

	TSharedRef<FInventoryDragDropOp> Op = FInventoryDragDropOp::New(
		ItemInSlot.InstanceID,
		ItemInSlot.StaticDataID,
		ViewModel,
		SlotType,
		Decorator,
		NewOffset,
		nullptr
	);

	return FReply::Handled().BeginDragDrop(Op);
}