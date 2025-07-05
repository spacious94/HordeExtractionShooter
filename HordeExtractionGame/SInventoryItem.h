// SInventoryItem.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "InventorySystemTypes.h"         // FItemEntry

class UInventoryViewModel;
class FInventoryDragDropOp;

/**
 * One icon-tile inside the inventory grid.
 * Handles drag start & supplies its own icon brush.
 */
class HORDEEXTRACTIONGAME_API SInventoryItem : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryItem)
		: _ItemEntry()
		, _ViewModel(nullptr)
		, _TileSize(64.f) {
		}
		SLATE_ARGUMENT(FItemEntry, ItemEntry)
		SLATE_ARGUMENT(UInventoryViewModel*, ViewModel)
		SLATE_ARGUMENT(float, TileSize)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~SInventoryItem();

	// --- drag source overrides
	virtual FReply OnMouseButtonDown(
		const FGeometry& MyGeometry,
		const FPointerEvent& MouseEvent) override;

	virtual FReply OnDragDetected(
		const FGeometry& MyGeometry,
		const FPointerEvent& MouseEvent) override;

	virtual void OnDragLeave(
		const FDragDropEvent& DragDropEvent) override;

	FPrimaryAssetId GetStaticDataID() const { return Item.StaticDataID; }

private:
	FItemEntry              Item;
	UInventoryViewModel* ViewModel = nullptr;
	float                   TileSize = 64.f;

	// Store a weak reference to the drag operation we created.
	TWeakPtr<FInventoryDragDropOp> DragDropOperation;

	const FSlateBrush* GetItemIcon() const;
};
