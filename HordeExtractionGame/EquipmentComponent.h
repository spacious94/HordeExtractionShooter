// EquipmentComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemTypes.h"
#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "EquipmentSystemTypes.h"
#include "EquipmentComponent.generated.h"

USTRUCT()
struct FEquippedHandles
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectHandles;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, const FItemInstance&, ItemInstance);

UCLASS(ClassGroup = (Custom), Blueprintable)
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

	// --- NEW: Function to handle dropping an item directly from a slot. ---
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void DropEquippedItem(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool GetEquippedItem(EEquipmentSlot Slot, FItemInstance& OutItem) const;

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

	UFUNCTION(BlueprintPure, Category = "Equipment | Server")
	TArray<FGameplayAbilitySpecHandle> GetAbilityHandlesForSlot(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Equipment | Server")
	TArray<FActiveGameplayEffectHandle> GetEffectHandlesForSlot(EEquipmentSlot Slot);

	void HandleEquip(EEquipmentSlot Slot, const FItemInstance& ItemInstance);
	void HandleUnequip(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnItemEquipped(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnItemUnequipped(EEquipmentSlot Slot, const FItemInstance& ItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(EEquipmentSlot Slot, const FGuid& ItemID);

	UFUNCTION(Server, Reliable)
	void Server_UnequipItem(EEquipmentSlot Slot);

	// --- NEW: Server RPC for the drop logic. ---
	UFUNCTION(Server, Reliable)
	void Server_DropEquippedItem(EEquipmentSlot Slot);

	UPROPERTY()
	TMap<EEquipmentSlot, TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY()
	TMap<EEquipmentSlot, FEquippedHandles> GrantedHandles;
};