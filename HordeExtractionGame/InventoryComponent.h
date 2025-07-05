// InventoryComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystemTypes.h"
#include "InventoryComponent.generated.h"

// Delegate for when an item is added, removed, or changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, const FItemEntry&, ItemEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChanged, const FItemEntry&, ItemEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, const FGuid&, ItemID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORDEEXTRACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(ReplicatedUsing = OnRep_InventoryList, BlueprintReadOnly, Category = "Inventory")
	FInventoryList InventoryList;

	UFUNCTION()
	void OnRep_InventoryList();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemChanged OnItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemRemoved OnItemRemoved;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Adds a new item to the inventory on the server. Finds the first available slot.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(const FPrimaryAssetId& ItemStaticID, int32 StackSize, int32 Durability);

	// Adds an item using a pre-existing FItemEntry, preserving its GUID.
	void AddItemFromEntry(const FItemEntry& ItemEntry, int32 StackSize, int32 Durability);

	// Removes an item from the inventory by its unique ID.
	void RemoveItemByID(const FGuid& ItemID);

	// Moves an item to a new grid location on the server.
	UFUNCTION(Server, Reliable)
	void Server_MoveItem(const FGuid& ItemID, int32 NewX, int32 NewY);

	UFUNCTION(Server, Reliable)
	void Server_RemoveItem(const FGuid& ItemID);
	
	UFUNCTION(Server, Reliable)
	void Server_SplitStack(const FGuid& ItemID, int32 AmountToSplit);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(const FGuid& ItemID);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(const FGuid& ItemID);

	// Returns a copy of all items in the inventory.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemEntry> GetAllItems() const;

	// Helper to check if a grid space is available for a given item size
	bool IsSpaceAvailable(FIntPoint TopLeft, FIntPoint Size, const FGuid& IgnoreItemID = FGuid()) const;

	// Helper function to find an empty slot for an item of a given size.
	FIntPoint FindFirstEmptySlotForSize(FIntPoint Size) const;

	// Helper to get item dimensions from its data asset
	FIntPoint GetItemSize(const FPrimaryAssetId& ItemStaticID) const;

protected:
	virtual void BeginPlay() override;

private:
	
};