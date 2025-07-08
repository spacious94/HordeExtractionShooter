// SInventoryGrid.cpp
#include "SInventoryGrid.h"
#include "SInventoryItem.h"
#include "InventoryViewModel.h"
#include "DragDropOps.h"
#include "HordeExtractionGame.h"
#include "HordeFunctionLibrary.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Colors/SColorBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInventoryGrid::Construct(const FArguments& InArgs)
{
	ViewModel = InArgs._ViewModel;
	TileSize = InArgs._TileSize;
	Columns = InArgs._Columns;
	Rows = InArgs._Rows;

	SAssignNew(GridPanel, SGridPanel);

	for (int32 Y = 0; Y < Rows; ++Y)
	{
		for (int32 X = 0; X < Columns; ++X)
		{
			GridPanel->AddSlot(X, Y)
				[
					SNew(SBox)
						.WidthOverride(TileSize)
						.HeightOverride(TileSize)
				];
		}
	}

	if (ViewModel)
	{
		ViewModel->OnInventoryChanged.AddSP(this, &SInventoryGrid::RefreshGrid);
	}

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				[
					GridPanel.ToSharedRef()
				]
				+ SOverlay::Slot()
				[
					SAssignNew(IndicatorCanvas, SCanvas)
				]
		];

	RefreshGrid();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SInventoryGrid::~SInventoryGrid()
{
	if (ViewModel)
	{
		ViewModel->OnInventoryChanged.RemoveAll(this);
	}
}

FIntPoint SInventoryGrid::ScreenToGrid(const FGeometry& Geo, FVector2D ScreenPos) const
{
	const FVector2D Local = Geo.AbsoluteToLocal(ScreenPos);
	int32 GX = FMath::Clamp(FMath::FloorToInt(Local.X / TileSize), 0, Columns - 1);
	int32 GY = FMath::Clamp(FMath::FloorToInt(Local.Y / TileSize), 0, Rows - 1);
	return FIntPoint(GX, GY);
}

void SInventoryGrid::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	if (TSharedPtr<FInventoryDragDropOp> Op = DragDropEvent.GetOperationAs<FInventoryDragDropOp>())
	{
		bIsDragInProgress = true;
		DraggedItemID = Op->ItemID;
		DraggedItemSize = UHordeFunctionLibrary::GetItemSize(Op->StaticDataID);
	}
}

void SInventoryGrid::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	bIsDragInProgress = false;
	if (IndicatorCanvas.IsValid())
	{
		IndicatorCanvas->ClearChildren();
	}
}

FReply SInventoryGrid::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	if (bIsDragInProgress && IndicatorCanvas.IsValid() && ViewModel)
	{
		IndicatorCanvas->ClearChildren();

		const TSharedPtr<FInventoryDragDropOp> Op = DragDropEvent.GetOperationAs<FInventoryDragDropOp>();
		if (!Op.IsValid()) return FReply::Unhandled();

		const FVector2D DecoratorScreenPosition = DragDropEvent.GetScreenSpacePosition() + Op->DecoratorOffset;
		const FIntPoint Cell = ScreenToGrid(MyGeometry, DecoratorScreenPosition);

		const bool bIsSpaceAvailable = ViewModel->IsInventorySpaceAvailable(Cell, DraggedItemSize, DraggedItemID);
		const FLinearColor IndicatorColor = bIsSpaceAvailable ? FLinearColor(0, 1, 0, 0.4f) : FLinearColor(1, 0, 0, 0.4f);

		for (int32 Y = 0; Y < DraggedItemSize.Y; ++Y)
		{
			for (int32 X = 0; X < DraggedItemSize.X; ++X)
			{
				IndicatorCanvas->AddSlot()
					.Position(FVector2D((Cell.X + X) * TileSize, (Cell.Y + Y) * TileSize))
					.Size(FVector2D(TileSize, TileSize))
					[
						SNew(SColorBlock)
							.Color(IndicatorColor)
					];
			}
		}
	}
	return FReply::Handled();
}

FReply SInventoryGrid::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	const TSharedPtr<FInventoryDragDropOp> Op = DragDropEvent.GetOperationAs<FInventoryDragDropOp>();
	if (!Op.IsValid() || !Op->ViewModel.IsValid())
	{
		return FReply::Unhandled();
	}

	bIsDragInProgress = false;
	if (IndicatorCanvas.IsValid())
	{
		IndicatorCanvas->ClearChildren();
	}

	if (Op->SourceSlot != EEquipmentSlot::None)
	{
		ViewModel->RequestUnequipItem(Op->SourceSlot);
		Op->bDropSucceeded = true;
		return FReply::Handled();
	}
	else
	{
		const FVector2D DecoratorScreenPosition = DragDropEvent.GetScreenSpacePosition() + Op->DecoratorOffset;
		const FIntPoint Cell = ScreenToGrid(MyGeometry, DecoratorScreenPosition);
		const FIntPoint ItemSize = UHordeFunctionLibrary::GetItemSize(Op->StaticDataID);

		if (ViewModel->IsInventorySpaceAvailable(Cell, ItemSize, Op->ItemID))
		{
			// Space is available, perform the move.
			ViewModel->MoveInventoryItem(Op->ItemID, Cell.X, Cell.Y);
			Op->bDropSucceeded = true;
		}

		// --- MODIFIED: We now ALWAYS handle the drop. ---
		// If space was not available, bDropSucceeded remains false, and the item
		// will be returned to its original slot by the drag operation's destructor.
		// By returning Handled() here, we prevent the "drop in world" fallback.
		return FReply::Handled();
	}
}

void SInventoryGrid::SetViewModel(UInventoryViewModel* InModel)
{
	if (ViewModel)
	{
		ViewModel->OnInventoryChanged.RemoveAll(this);
	}
	ViewModel = InModel;
	if (ViewModel)
	{
		ViewModel->OnInventoryChanged.AddSP(this, &SInventoryGrid::RefreshGrid);
	}
	RefreshGrid();
}

void SInventoryGrid::RefreshGrid()
{
	if (!GridPanel.IsValid() || !ViewModel)
	{
		return;
	}

	GridPanel->ClearChildren();

	for (int32 Y = 0; Y < Rows; ++Y)
	{
		for (int32 X = 0; X < Columns; ++X)
		{
			GridPanel->AddSlot(X, Y)
				[
					SNew(SBox)
						.WidthOverride(TileSize)
						.HeightOverride(TileSize)
				];
		}
	}

	const TArray<FItemEntry> Items = ViewModel->GetInventoryItems();
	for (const FItemEntry& It : Items)
	{
		FIntPoint ItemSize = UHordeFunctionLibrary::GetItemSize(It.StaticDataID);
		GridPanel->AddSlot(It.GridX, It.GridY)
			.ColumnSpan(ItemSize.X)
			.RowSpan(ItemSize.Y)
			[
				SNew(SInventoryItem)
					.ItemEntry(It)
					.ViewModel(ViewModel)
					.TileSize(TileSize)
			];
	}
}

FVector2D SInventoryGrid::ComputeDesiredSize(float) const
{
	return FVector2D(Columns * TileSize, Rows * TileSize);
}