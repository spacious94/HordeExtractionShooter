// EquipmentComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemTypes.h"
#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "EquipmentSystemTypes.h"
#include "EquipmentComponent.generated.h"

// A simple struct to hold all the handles related to a single equipped item.
USTRUCT()
struct FEquippedHandles
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectHandles;
};

// Delegate for when an item is equipped or unequipped
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, const FItemInstance&, ItemInstance);

UCLASS(ClassGroup=(Custom), Blueprintable)
class HORDEEXTRACTIONGAME_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Equipment")
	FEquippedList EquippedList;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(EEquipmentSlot Slot, const FGuid& ItemID);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipItem(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool GetEquippedItem(EEquipmentSlot Slot, FItemInstance& OutItem) const;

	// --- Blueprint Callable Functions for managing spawned actors and handles ---
	UFUNCTION(BlueprintCallable, Category = "Equipment | Server")
	void StoreSpawnedActor(EEquipmentSlot Slot, AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Equipment | Server")
	void StoreAbilityHandle(EEquipmentSlot Slot, const FGameplayAbilitySpecHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "Equipment | Server")
	void StoreEffectHandle(EEquipmentSlot Slot, const FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "Equipment | Server")
	void ClearAllHandlesForSlot(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Equipment | Server")
	AActor* GetSpawnedActorForSlot(EEquipmentSlot Slot);

	// Because the handle structs aren't Blueprint types, we need helper functions to get the arrays.
	UFUNCTION(BlueprintPure, Category = "Equipment | Server")
	TArray<FGameplayAbilitySpecHandle> GetAbilityHandlesForSlot(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Equipment | Server")
	TArray<FActiveGameplayEffectHandle> GetEffectHandlesForSlot(EEquipmentSlot Slot);

	// These functions are now responsible for calling the Blueprint events.
	void HandleEquip(EEquipmentSlot Slot, const FItemInstance& ItemInstance);
	void HandleUnequip(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

protected:
	// These are now Blueprint events that you will implement in a Blueprint subclass.
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnItemEquipped(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnItemUnequipped(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(EEquipmentSlot Slot, const FGuid& ItemID);

	UFUNCTION(Server, Reliable)
	void Server_UnequipItem(EEquipmentSlot Slot);

	// These TMaps are not replicated. They live on the server and keep track of what has been given to the player.
	UPROPERTY()
	TMap<EEquipmentSlot, TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY()
	TMap<EEquipmentSlot, FEquippedHandles> GrantedHandles;
};
