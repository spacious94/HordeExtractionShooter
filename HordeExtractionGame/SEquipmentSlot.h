// SEquipmentSlot.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "EquipmentSystemTypes.h" // For EEquipmentSlot

#include "ItemTypes.h"

class UInventoryViewModel;

class HORDEEXTRACTIONGAME_API SEquipmentSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEquipmentSlot)
		: _ViewModel(nullptr)
		, _SlotType(EEquipmentSlot::None)
		, _TileSize(64.f)
	{}
		SLATE_ARGUMENT(UInventoryViewModel*, ViewModel)
		SLATE_ARGUMENT(EEquipmentSlot, SlotType)
		SLATE_ARGUMENT(float, TileSize)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// This is the key function that will handle the drop logic in C++.
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	// --- Drag/Drop Source Overrides ---
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void RefreshSlot(EEquipmentSlot Slot, const FItemInstance& Item);

	void SetViewModel(UInventoryViewModel* InViewModel);

private:
	UInventoryViewModel* ViewModel = nullptr;
	EEquipmentSlot SlotType = EEquipmentSlot::None;
	float TileSize = 64.f;

	TSharedPtr<class SImage> ItemIcon;
};
