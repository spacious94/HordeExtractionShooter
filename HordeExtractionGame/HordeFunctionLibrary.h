// HordeFunctionLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemTypes.h"
#include "HordeFunctionLibrary.generated.h"

class UAbilitySystemComponent; // Forward Declaration

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
};
