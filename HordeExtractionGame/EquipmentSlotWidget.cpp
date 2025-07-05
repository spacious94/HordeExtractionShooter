// EquipmentSlotWidget.cpp
#include "EquipmentSlotWidget.h"
#include "SEquipmentSlot.h"
#include "InventoryViewModel.h"
#include "Widgets/SNullWidget.h"

TSharedRef<SWidget> UEquipmentSlotWidget::RebuildWidget()
{
	SAssignNew(EquipmentSlot, SEquipmentSlot)
		.ViewModel(ViewModel)
		.SlotType(SlotType);

	return EquipmentSlot.IsValid() ? EquipmentSlot.ToSharedRef() : SNullWidget::NullWidget;
}

void UEquipmentSlotWidget::AssignViewModel(UInventoryViewModel* InViewModel)
{
	ViewModel = InViewModel;
	if (EquipmentSlot.IsValid())
	{
		EquipmentSlot->SetViewModel(ViewModel);
		
		// Also trigger a refresh to get the initial state.
		FItemInstance InitialItem;
		if (ViewModel)
		{
			ViewModel->GetEquippedItemForSlot(SlotType, InitialItem);
		}
		EquipmentSlot->RefreshSlot(SlotType, InitialItem);
	}
}

void UEquipmentSlotWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	EquipmentSlot.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}
