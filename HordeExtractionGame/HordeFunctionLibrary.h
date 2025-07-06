// HordeFunctionLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemTypes.h"
#include "HordeFunctionLibrary.generated.h"

class UAbilitySystemComponent;
class UDragDropOperation;
class UInventoryViewModel;

UINTERFACE(MinimalAPI, Blueprintable)
class UPickupInitializationInterface : public UInterface
{
	GENERATED_BODY()
};

class IPickupInitializationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void InitializePickup(const FItemInstance& ItemData);
};

UCLASS()
class HORDEEXTRACTIONGAME_API UHordeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static FIntPoint GetItemSize(const FPrimaryAssetId& ItemStaticID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void InitializePickupActor(AActor* PickupActor, const FItemInstance& ItemInstance);

	/**
	 * Exposes the non-Blueprint-callable InitAbilityActorInfo function to Blueprints.
	 * This is the correct way to initialize an ASC on an actor that was added in Blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	static void InitializeAbilitySystem(UAbilitySystemComponent* ASC, AActor* OwnerActor, AActor* AvatarActor);

	/**
	 * Safely gets the payload from an FInventoryDragDropOp.
	 * @param Operation The Drag and Drop operation from a UMG OnDrop event.
	 * @param OutViewModel The ViewModel associated with the drag operation.
	 * @param OutItemID The unique ID of the item being dragged.
	 * @return True if the operation was a valid FInventoryDragDropOp and the payload was extracted.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI | Drag and Drop", meta = (DeterminesOutputType = "Operation"))
	static bool GetInventoryDragDropPayload(UDragDropOperation* Operation, UInventoryViewModel*& OutViewModel, FGuid& OutItemID);

	UFUNCTION(BlueprintCallable, Category = "UI | Drag and Drop")
	static void SetDragDropSucceeded(UDragDropOperation* Operation, bool bSucceeded);
};
