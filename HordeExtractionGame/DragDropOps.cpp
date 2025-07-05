
// DragDropOps.cpp
#include "DragDropOps.h"
#include "InventoryViewModel.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWidget.h"
#include "HordeExtractionGame.h"

#include "SInventoryItem.h"

FInventoryDragDropOp::~FInventoryDragDropOp()
{
	// If the drop was not handled by any widget, it's considered cancelled.
	if (!bWasDropHandled)
	{
		// If the source item widget is still valid, make it visible again. This preserves the fix for the window-close crash.
		if (SourceItem.IsValid())
		{
			SourceItem.Pin()->SetVisibility(EVisibility::Visible);
		}
	}
	// If the drop WAS handled, we do nothing. The source widget will be destroyed by the inventory grid refresh.
}

TSharedRef<FInventoryDragDropOp> FInventoryDragDropOp::New(const FGuid& InItemID, const FPrimaryAssetId& InStaticDataID, UInventoryViewModel* InViewModel, TSharedPtr<SWidget> InDecorator, FVector2D InDecoratorOffset, TSharedPtr<SInventoryItem> InSourceItem)
{
	TSharedRef<FInventoryDragDropOp> Operation = MakeShared<FInventoryDragDropOp>();
	Operation->ItemID = InItemID;
	Operation->StaticDataID = InStaticDataID;
	Operation->ViewModel = InViewModel;
	Operation->Decorator = InDecorator;
	Operation->DecoratorOffset = InDecoratorOffset;
	Operation->SourceItem = InSourceItem;
	
	Operation->Construct();

	return Operation;
}

void FInventoryDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	// This is the key change. We store the result of the drop.
	bWasDropHandled = bDropWasHandled;

	// If the drop was not handled by any widget, we can assume the player wants to drop it into the world.
	if (!bWasDropHandled && ViewModel.IsValid())
	{
		ViewModel->RequestDropItem(ItemID);
	}

	// The base class OnDrop is what calls the destructor, so we call it last.
	FGameDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}

TSharedPtr<SWidget> FInventoryDragDropOp::GetDefaultDecorator() const
{
	return Decorator;
}

FVector2D FInventoryDragDropOp::GetDecoratorPosition() const
{
	// This is the crucial part. We get the current cursor position and apply our offset.
	return FSlateApplication::Get().GetCursorPos() + DecoratorOffset;
}
