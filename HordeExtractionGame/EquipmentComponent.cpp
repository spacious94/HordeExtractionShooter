// EquipmentComponent.cpp
#include "EquipmentComponent.h"
#include "GASPlayerState.h"
#include "InventoryComponent.h"
#include "ItemDatabaseComponent.h"
#include "Net/UnrealNetwork.h"

#include "HordeExtractionGame.h" // For LogInventoryUI

UEquipmentComponent::UEquipmentComponent()
{
	SetIsReplicatedByDefault(true);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	EquippedList.OwningComponent = this;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UEquipmentComponent, EquippedList, COND_OwnerOnly);
}

void UEquipmentComponent::EquipItem(EEquipmentSlot Slot, const FGuid& ItemID)
{
	UE_LOG(LogInventoryUI, Log, TEXT("UEquipmentComponent::EquipItem - Calling Server RPC."));
	Server_EquipItem(Slot, ItemID);
}

void UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
	Server_UnequipItem(Slot);
}

bool UEquipmentComponent::GetEquippedItem(EEquipmentSlot Slot, FItemInstance& OutItem) const
{
	for (const FEquippedItem& EquippedItem : EquippedList.Items)
	{
		if (EquippedItem.Slot == Slot)
		{
			OutItem = EquippedItem.Item;
			return true;
		}
	}
	return false;
}

void UEquipmentComponent::Server_EquipItem_Implementation(EEquipmentSlot Slot, const FGuid& ItemID)
{
	UE_LOG(LogInventoryUI, Log, TEXT("UEquipmentComponent::Server_EquipItem_Implementation - EXECUTING ON SERVER."));
	AGASPlayerState* PlayerState = GetOwner<AGASPlayerState>();
	if (!PlayerState || !PlayerState->HasAuthority()) return;

	UInventoryComponent* InventoryComp = PlayerState->GetInventoryComponent();
	UItemDatabaseComponent* DatabaseComp = PlayerState->GetItemDatabaseComponent();
	if (!InventoryComp || !DatabaseComp)
	{
		UE_LOG(LogInventoryUI, Error, TEXT("Server_EquipItem - FAILED, Inventory or Database component was null."));
		return;
	}

	const FItemInstance* ItemInstanceToEquip = DatabaseComp->GetItemInstance(ItemID);

	if (ItemInstanceToEquip)
	{
		UE_LOG(LogInventoryUI, Log, TEXT("Server_EquipItem - SUCCESS, Found item in database. Proceeding with equip."));
		// Unequip any existing item first.
		Server_UnequipItem(Slot);

		// Now remove the item from the inventory component.
		InventoryComp->RemoveItemByID(ItemID);

		FEquippedItem NewEquippedItem;
		NewEquippedItem.Slot = Slot;
		NewEquippedItem.Item = *ItemInstanceToEquip;
		
		EquippedList.Items.Add(NewEquippedItem);
		EquippedList.MarkItemDirty(EquippedList.Items.Last());

		HandleEquip(Slot, *ItemInstanceToEquip);
		OnEquipmentChanged.Broadcast(Slot, *ItemInstanceToEquip);
	}
	else
	{
		UE_LOG(LogInventoryUI, Warning, TEXT("Server_EquipItem - FAILED, could not find ItemID %s in database."), *ItemID.ToString());
	}
}

void UEquipmentComponent::Server_UnequipItem_Implementation(EEquipmentSlot Slot)
{
	AGASPlayerState* PlayerState = GetOwner<AGASPlayerState>();
	if (!PlayerState || !PlayerState->HasAuthority()) return;

	UInventoryComponent* InventoryComp = PlayerState->GetInventoryComponent();
	UItemDatabaseComponent* DatabaseComp = PlayerState->GetItemDatabaseComponent();
	if (!InventoryComp || !DatabaseComp) return;

	int32 FoundIndex = EquippedList.Items.IndexOfByPredicate(
		[&Slot](const FEquippedItem& Item){ return Item.Slot == Slot; });

	if (FoundIndex != INDEX_NONE)
	{
		FItemInstance ItemToUnequip_Instance = EquippedList.Items[FoundIndex].Item;

		// Now we can get the StaticDataID directly from the instance
		FIntPoint ItemSize = InventoryComp->GetItemSize(ItemToUnequip_Instance.StaticDataID);
		if (InventoryComp->FindFirstEmptySlotForSize(ItemSize).X != -1)
		{
			EquippedList.Items.RemoveAt(FoundIndex);
			EquippedList.MarkArrayDirty();
			
			// We need to get the stack size and durability from somewhere.
			// For now, we'll assume 1 and 100. This data should also be added to FItemInstance.
			InventoryComp->AddItem(ItemToUnequip_Instance.StaticDataID, 1, 100);

			HandleUnequip(Slot, ItemToUnequip_Instance);
			OnEquipmentChanged.Broadcast(Slot, FItemInstance()); // Broadcast empty instance on unequip
		}
	}
}

void UEquipmentComponent::StoreSpawnedActor(EEquipmentSlot Slot, AActor* InActor)
{
	if (GetOwner()->HasAuthority())
	{
		SpawnedActors.Add(Slot, InActor);
	}
}

void UEquipmentComponent::StoreAbilityHandle(EEquipmentSlot Slot, const FGameplayAbilitySpecHandle& Handle)
{
	if (GetOwner()->HasAuthority())
	{
		GrantedHandles.FindOrAdd(Slot).AbilityHandles.Add(Handle);
	}
}

void UEquipmentComponent::StoreEffectHandle(EEquipmentSlot Slot, const FActiveGameplayEffectHandle& Handle)
{
	if (GetOwner()->HasAuthority())
	{
		GrantedHandles.FindOrAdd(Slot).EffectHandles.Add(Handle);
	}
}

void UEquipmentComponent::ClearAllHandlesForSlot(EEquipmentSlot Slot)
{
	if (GetOwner()->HasAuthority())
	{
		SpawnedActors.Remove(Slot);
		GrantedHandles.Remove(Slot);
	}
}

AActor* UEquipmentComponent::GetSpawnedActorForSlot(EEquipmentSlot Slot)
{
	if (const TObjectPtr<AActor>* FoundActor = SpawnedActors.Find(Slot))
	{
		return *FoundActor;
	}
	return nullptr;
}

TArray<FGameplayAbilitySpecHandle> UEquipmentComponent::GetAbilityHandlesForSlot(EEquipmentSlot Slot)
{
	if (const FEquippedHandles* FoundHandles = GrantedHandles.Find(Slot))
	{
		return FoundHandles->AbilityHandles;
	}
	return TArray<FGameplayAbilitySpecHandle>();
}

TArray<FActiveGameplayEffectHandle> UEquipmentComponent::GetEffectHandlesForSlot(EEquipmentSlot Slot)
{
	if (const FEquippedHandles* FoundHandles = GrantedHandles.Find(Slot))
	{
		return FoundHandles->EffectHandles;
	}
	return TArray<FActiveGameplayEffectHandle>();
}

void UEquipmentComponent::HandleEquip(EEquipmentSlot Slot, const FItemInstance& ItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		// This logic should only run on the server.
		ClearAllHandlesForSlot(Slot);
		OnItemEquipped(Slot, ItemInstance);
	}
	// Note: The client-side visual update is handled by the OnEquipmentChanged delegate
	// which is fired by the FastArraySerializer's PostReplicatedAdd callback.
}

void UEquipmentComponent::HandleUnequip(EEquipmentSlot Slot, const FItemInstance& ItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		// This logic should only run on the server.
		OnItemUnequipped(Slot, ItemInstance);
		ClearAllHandlesForSlot(Slot);
	}
	// Note: The client-side visual update is handled by the OnEquipmentChanged delegate
	// which is fired by the FastArraySerializer's PreReplicatedRemove callback.
}
