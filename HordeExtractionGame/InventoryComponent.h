// Copyright Your Name or Company Name. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystemTypes.h" 
#include "InventoryComponent.generated.h"

// Delegate signatures for reacting to inventory changes in Blueprints (e.g., to update UI)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, const FItemEntry&, NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChanged, const FItemEntry&, ChangedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, const FGuid&, RemovedItemGUID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORDEEXTRACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// Blueprint-assignable delegates for UI to bind to.
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnItemChanged OnItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnItemRemoved OnItemRemoved;

	//~ Begin UActorComponent Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
	//~ End UActorComponent Interface

	/** Adds an item to the inventory. MUST be called on the server. */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Actions")
	void AddItem(const FPrimaryAssetId& ItemStaticID, int32 StackSize, int32 Durability);

	/** Moves an item to a new grid location. MUST be called on the server. */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory|Actions")
	void Server_MoveItem(const FGuid& ItemID, int32 NewX, int32 NewY);

	/** Completely removes an item from the inventory. MUST be called on the server. */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory|Actions")
	void Server_RemoveItem(const FGuid& ItemID);

	/** Splits a stack into a new stack. MUST be called on the server. */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory|Actions")
	void Server_SplitStack(const FGuid& ItemID, int32 AmountToSplit);

	/** Equips an item from the inventory. MUST be called on the server. */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory|Actions")
	void Server_EquipItem(const FGuid& ItemID);

	/** Gets a copy of all item entries currently in the inventory. */
	UFUNCTION(BlueprintPure, Category = "Inventory|Read")
	TArray<FItemEntry> GetAllItems() const;

private:
	UPROPERTY(Replicated, DisplayName = "Inventory")
	FInventoryList InventoryList;
};