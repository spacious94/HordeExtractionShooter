// InventoryViewModel.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemTypes.h"
#include "EquipmentSystemTypes.h" // Required for EEquipmentSlot
#include "InventoryViewModel.generated.h"

class UInventoryComponent;
class UEquipmentComponent;

/**
 * The ViewModel for the Inventory UI. Acts as a bridge between the data components and the Slate UI.
 */
UCLASS(BlueprintType)
class HORDEEXTRACTIONGAME_API UInventoryViewModel : public UObject
{
	GENERATED_BODY()

public:
	// Initializes the ViewModel with the data sources
	UFUNCTION(BlueprintCallable, Category = "ViewModel")
	void Initialize(UInventoryComponent* InInventoryComponent, UEquipmentComponent* InEquipmentComponent);

	// --- Inventory Functions ---
	TArray<FItemEntry> GetInventoryItems() const;
	void MoveInventoryItem(const FGuid& ItemID, int32 NewX, int32 NewY);
	bool IsInventorySpaceAvailable(FIntPoint TopLeft, FIntPoint Size, const FGuid& IgnoreItemID) const;
	void RequestDropItem(const FGuid& ItemID);

	// --- Equipment Functions ---
	UFUNCTION(BlueprintCallable, Category = "ViewModel | Equipment")
	void RequestEquipItem(EEquipmentSlot Slot, const FGuid& ItemID);
	
	UFUNCTION(BlueprintCallable, Category = "ViewModel | Equipment")
	void RequestUnequipItem(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "ViewModel | Equipment")
	bool GetEquippedItemForSlot(EEquipmentSlot Slot, FItemInstance& OutItem) const;

	// --- Delegates for UI Updates ---
	DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);
	FOnInventoryChanged OnInventoryChanged;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, const FItemInstance&);
	FOnEquipmentChanged OnEquipmentChanged;

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	// Handlers for component data changes
	UFUNCTION()
	void HandleInventoryChanged(const FItemEntry& ItemEntry);
	UFUNCTION()
	void HandleItemRemoved(const FGuid& ItemID);
	UFUNCTION()
	void HandleEquipmentChanged(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

	bool bIsInitialized = false;
};
