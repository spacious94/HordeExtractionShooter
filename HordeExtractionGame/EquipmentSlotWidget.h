// EquipmentSlotWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "EquipmentSystemTypes.h" // For EEquipmentSlot
#include "EquipmentSlotWidget.generated.h"

class SEquipmentSlot;
class UInventoryViewModel;

UCLASS()
class HORDEEXTRACTIONGAME_API UEquipmentSlotWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (ExposeOnSpawn = true))
	TObjectPtr<UInventoryViewModel> ViewModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (ExposeOnSpawn = true))
	EEquipmentSlot SlotType;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void AssignViewModel(UInventoryViewModel* InViewModel);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SEquipmentSlot> EquipmentSlot;
};
