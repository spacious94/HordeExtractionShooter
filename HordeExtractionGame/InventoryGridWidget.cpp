// InventoryGridWidget.cpp

#include "InventoryGridWidget.h"
#include "SInventoryGrid.h"
#include "InventoryViewModel.h"
#include "Widgets/SNullWidget.h"
#include "HordeExtractionGame.h"

TSharedRef<SWidget> UInventoryGridWidget::RebuildWidget()
{
	// We ALWAYS build the Slate grid, even if ViewModel is still null.
	// That lets AssignViewModel push data in later without needing a full rebuild.
	SAssignNew(InventoryGrid, SInventoryGrid)
		.ViewModel(ViewModel);

	if (ViewModel)
	{
		UE_LOG(LogInventoryUI, Log,
			TEXT("RebuildWidget: ViewModel=%s � built live SInventoryGrid"),
			*GetNameSafe(ViewModel));
	}
	else
	{
		UE_LOG(LogInventoryUI, Log,
			TEXT("RebuildWidget: ViewModel is NULL at first construction. "
				"Grid will refresh after AssignViewModel is called."));
	}

	return InventoryGrid.IsValid() ? InventoryGrid.ToSharedRef()
		: SNullWidget::NullWidget;
}

void UInventoryGridWidget::AssignViewModel(UInventoryViewModel* InViewModel)
{
	UE_LOG(LogInventoryUI, Log,
		TEXT("AssignViewModel called.  InViewModel=%s  (prev=%s)"),
		*GetNameSafe(InViewModel),
		*GetNameSafe(ViewModel));

	ViewModel = InViewModel;

	if (InventoryGrid.IsValid())
	{
		InventoryGrid->SetViewModel(ViewModel);
		InventoryGrid->RefreshGrid();

		UE_LOG(LogInventoryUI, Log,
			TEXT("AssignViewModel: pushed model into existing Slate grid and "
				"triggered RefreshGrid()"));
	}
	else
	{
		UE_LOG(LogInventoryUI, Error,
			TEXT("AssignViewModel: InventoryGrid slate object was invalid!"));
	}
}

void UInventoryGridWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	UE_LOG(LogInventoryUI, Log, TEXT("ReleaseSlateResources on UInventoryGridWidget"));
	InventoryGrid.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}
