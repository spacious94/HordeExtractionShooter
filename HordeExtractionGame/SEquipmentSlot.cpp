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
			// TODO: Add validation logic here to check if the item is valid for this slot.
			ViewModel->RequestEquipItem(SlotType, Op->ItemID);
			Op->bDropSucceeded = true;
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

FReply SEquipmentSlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// Check if there's actually an item in this slot to drag
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

	// Create the decorator widget
	TSharedRef<SWidget> Decorator = SNew(SBox)
		.WidthOverride(TileSize)
		.HeightOverride(TileSize)
		[
			SNew(SImage)
			.Image(Brush) // Use the brush we just fetched
			.ColorAndOpacity(FLinearColor(1, 1, 1, 0.75f))
		];

	// Get the mouse position in the local space of the widget being dragged.
	const FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const FVector2D NewOffset = -LocalMousePosition;

	// Immediately request to unequip the item from the model.
	// The UI will update automatically via the OnEquipmentChanged delegate.
	ViewModel->RequestUnequipItem(SlotType);

	// Create the drag operation
	TSharedRef<FInventoryDragDropOp> Op = FInventoryDragDropOp::New(
		ItemInSlot.InstanceID,
		ItemInSlot.StaticDataID,
		ViewModel,
		Decorator,
		NewOffset,
		nullptr // We don't need a source item widget for equipment slots
	);

	return FReply::Handled().BeginDragDrop(Op);
}

