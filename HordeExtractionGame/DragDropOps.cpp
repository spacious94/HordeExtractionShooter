
// DragDropOps.cpp
#include "DragDropOps.h"
#include "InventoryViewModel.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWidget.h"
#include "HordeExtractionGame.h"

#include "SInventoryItem.h"

FInventoryDragDropOp::~FInventoryDragDropOp()
{
	if (!bDropSucceeded)
	{
		if (TSharedPtr<SInventoryItem> SourceItemPtr = SourceItem.Pin())
		{
			SourceItemPtr->SetVisibility(EVisibility::Visible);
		}
	}
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
	// If the drop was not handled by any widget, and our success flag has not been set,
	// then this is a "drop in world" event.
	if (!bDropWasHandled && !bDropSucceeded && ViewModel.IsValid())
	{
		ViewModel->RequestDropItem(ItemID);
		// A world drop is a successful outcome, so we set the flag.
		bDropSucceeded = true; 
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
