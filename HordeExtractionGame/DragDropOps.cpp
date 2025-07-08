// DragDropOps.cpp
#include "DragDropOps.h"
#include "InventoryViewModel.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWidget.h"
#include "HordeExtractionGame.h"

#include "SInventoryItem.h"

FInventoryDragDropOp::~FInventoryDragDropOp()
{
	// REVERTED: The destructor only needs to handle making the source widget
	// visible again if a drop was cancelled. All success logic is now on the server.
	if (!bDropSucceeded)
	{
		if (TSharedPtr<SInventoryItem> SourceItemPtr = SourceItem.Pin())
		{
			SourceItemPtr->SetVisibility(EVisibility::Visible);
		}
	}
}

TSharedRef<FInventoryDragDropOp> FInventoryDragDropOp::New(
	const FGuid& InItemID,
	const FPrimaryAssetId& InStaticDataID,
	UInventoryViewModel* InViewModel,
	EEquipmentSlot InSourceSlot,
	TSharedPtr<SWidget> InDecorator,
	FVector2D InDecoratorOffset,
	TSharedPtr<SInventoryItem> InSourceItem)
{
	TSharedRef<FInventoryDragDropOp> Operation = MakeShared<FInventoryDragDropOp>();
	Operation->ItemID = InItemID;
	Operation->StaticDataID = InStaticDataID;
	Operation->ViewModel = InViewModel;
	Operation->SourceSlot = InSourceSlot;
	Operation->Decorator = InDecorator;
	Operation->DecoratorOffset = InDecoratorOffset;
	Operation->SourceItem = InSourceItem;

	Operation->Construct();

	return Operation;
}

void FInventoryDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	if (!bDropWasHandled && !bDropSucceeded && ViewModel.IsValid())
	{
		// MODIFIED: We now pass the SourceSlot to RequestDropItem.
		ViewModel->RequestDropItem(ItemID, SourceSlot);
		bDropSucceeded = true;
	}

	FGameDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}

TSharedPtr<SWidget> FInventoryDragDropOp::GetDefaultDecorator() const
{
	return Decorator;
}

FVector2D FInventoryDragDropOp::GetDecoratorPosition() const
{
	return FSlateApplication::Get().GetCursorPos() + DecoratorOffset;
}