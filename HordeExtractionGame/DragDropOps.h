// DragDropOps.h
#pragma once

#include "CoreMinimal.h"
#include "Input/DragAndDrop.h"
#include "InventorySystemTypes.h"
#include "EquipmentSystemTypes.h"

class UInventoryViewModel;
class SWidget;
class SInventoryItem;

class HORDEEXTRACTIONGAME_API FInventoryDragDropOp : public FGameDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FInventoryDragDropOp, FGameDragDropOperation)

		~FInventoryDragDropOp();

	FGuid ItemID;
	FPrimaryAssetId StaticDataID;
	TWeakObjectPtr<UInventoryViewModel> ViewModel;

	EEquipmentSlot SourceSlot;

	TSharedPtr<SWidget> Decorator;
	FVector2D DecoratorOffset;

	TWeakPtr<SInventoryItem> SourceItem;

	bool bDropSucceeded = false;

	// --- NEW: Add a flag to track if the drop was handled by the world. ---
	bool bDropHandledByWorld = false;

	static TSharedRef<FInventoryDragDropOp> New(
		const FGuid& InItemID,
		const FPrimaryAssetId& InStaticDataID,
		UInventoryViewModel* InViewModel,
		EEquipmentSlot InSourceSlot,
		TSharedPtr<SWidget> InDecorator,
		FVector2D InDecoratorOffset,
		TSharedPtr<SInventoryItem> InSourceItem
	);

	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;
	virtual FVector2D GetDecoratorPosition() const override;

private:
	bool bWasDropHandled = false;
};