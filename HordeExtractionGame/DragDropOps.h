
// DragDropOps.h
#pragma once

#include "CoreMinimal.h"
#include "Input/DragAndDrop.h"
#include "InventorySystemTypes.h"

class UInventoryViewModel;
class SWidget;
class SInventoryItem;

class HORDEEXTRACTIONGAME_API FInventoryDragDropOp : public FGameDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FInventoryDragDropOp, FGameDragDropOperation)

	~FInventoryDragDropOp(); // Destructor for cleanup

	FGuid ItemID;
	FPrimaryAssetId StaticDataID;
	TWeakObjectPtr<UInventoryViewModel> ViewModel;
	
	TSharedPtr<SWidget> Decorator;
	FVector2D DecoratorOffset;

	TWeakPtr<SInventoryItem> SourceItem;

	bool bDropSucceeded = false;

	static TSharedRef<FInventoryDragDropOp> New(const FGuid& InItemID, const FPrimaryAssetId& InStaticDataID, UInventoryViewModel* InViewModel, TSharedPtr<SWidget> InDecorator, FVector2D InDecoratorOffset, TSharedPtr<SInventoryItem> InSourceItem);

	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;
	virtual FVector2D GetDecoratorPosition() const override;

private:
	bool bWasDropHandled = false;
};
