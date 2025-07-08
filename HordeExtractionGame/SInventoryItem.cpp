// SInventoryItem.cpp
#include "SInventoryItem.h"
#include "BrushCache.h"
#include "DragDropOps.h"
#include "HordeFunctionLibrary.h"
#include "InventoryViewModel.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Framework/Application/SlateApplication.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInventoryItem::Construct(const FArguments& InArgs)
{
	Item = InArgs._ItemEntry;
	ViewModel = InArgs._ViewModel;
	TileSize = InArgs._TileSize;

	ChildSlot
		[
			SNew(SBox)
				.WidthOverride(TileSize)
				.HeightOverride(TileSize)
				[
					SNew(SImage)
						.Image(this, &SInventoryItem::GetItemIcon)
				]
		];
}

SInventoryItem::~SInventoryItem()
{
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SInventoryItem::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}
	return FReply::Unhandled();
}

FReply SInventoryItem::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SetVisibility(EVisibility::Collapsed);

	FIntPoint ItemSize = UHordeFunctionLibrary::GetItemSize(Item.StaticDataID);
	FVector2D WidgetSize = FVector2D(ItemSize.X * TileSize, ItemSize.Y * TileSize);

	TSharedRef<SWidget> Decorator = SNew(SBox)
		.WidthOverride(WidgetSize.X)
		.HeightOverride(WidgetSize.Y)
		[
			SNew(SImage)
				.Image(GetItemIcon())
				.ColorAndOpacity(FLinearColor(1, 1, 1, 0.75f))
		];

	const FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const FVector2D NewOffset = -LocalMousePosition;

	// --- MODIFIED: Update the factory function call ---
	TSharedRef<FInventoryDragDropOp> Op = FInventoryDragDropOp::New(
		Item.UniqueID,
		Item.StaticDataID,
		ViewModel,
		EEquipmentSlot::None, // This drag did NOT come from an equipment slot.
		Decorator,
		NewOffset,
		SharedThis(this)
	);

	return FReply::Handled().BeginDragDrop(Op);
}

void SInventoryItem::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	SCompoundWidget::OnDragLeave(DragDropEvent);
}

const FSlateBrush* SInventoryItem::GetItemIcon() const
{
	if (UBrushCache* Cache = UBrushCache::Get())
	{
		return Cache->GetBrush(Item.StaticDataID).Get();
	}
	return nullptr;
}