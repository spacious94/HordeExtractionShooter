// InventoryGridWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "InventoryGridWidget.generated.h"

class SInventoryGrid;
class UInventoryViewModel;

/**
 * Lightweight UWidget that wraps an SInventoryGrid.
 * You may place it in the UMG designer; afterwards call AssignViewModel()
 * in Construct to inject the View-Model.
 */
UCLASS()
class HORDEEXTRACTIONGAME_API UInventoryGridWidget : public UWidget
{
	GENERATED_BODY()

public:
	/** If you spawn the widget from code you can still pass the VM on spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory",
		meta = (ExposeOnSpawn = true))
	TObjectPtr<UInventoryViewModel> ViewModel;

	/** Called from Blueprint after the View-Model exists. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AssignViewModel(UInventoryViewModel* InViewModel);

	// UWidget interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SInventoryGrid> InventoryGrid;
};
