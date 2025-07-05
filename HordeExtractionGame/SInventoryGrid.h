// SInventoryGrid.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "InventorySystemTypes.h"     // FItemEntry

class UInventoryViewModel;
class SGridPanel;
class SCanvas;

/**
 * Slate grid that hosts SInventoryItem tiles and handles drop targets.
 */
class HORDEEXTRACTIONGAME_API SInventoryGrid : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryGrid)
		: _ViewModel(nullptr)
		, _TileSize(64.f)
		, _Columns(12)
		, _Rows(6) {
		}
		SLATE_ARGUMENT(UInventoryViewModel*, ViewModel)
		SLATE_ARGUMENT(float, TileSize)
		SLATE_ARGUMENT(int32, Columns)
		SLATE_ARGUMENT(int32, Rows)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~SInventoryGrid();

	// accept drop
	virtual FReply OnDrop(const FGeometry&, const FDragDropEvent&) override;
	// Allow the grid itself to accept a drag hovering above it
	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FVector2D ComputeDesiredSize(float) const override;

	void SetViewModel(UInventoryViewModel* InModel);
	void RefreshGrid();

private:
	UInventoryViewModel* ViewModel = nullptr;
	TSharedPtr<SGridPanel> GridPanel;
	TSharedPtr<SCanvas> IndicatorCanvas;
	float  TileSize = 64.f;
	int32  Columns = 12;
	int32  Rows = 6;

	// State for the current drag operation
	bool bIsDragInProgress = false;
	FGuid DraggedItemID;
	FIntPoint DraggedItemSize;

	// helper converts screen pos ? grid coords
	FIntPoint ScreenToGrid(const FGeometry& Geo, FVector2D ScreenPos) const;
};
