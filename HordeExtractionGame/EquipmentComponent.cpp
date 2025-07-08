// EquipmentComponent.cpp
#include "EquipmentComponent.h"
#include "GASPlayerState.h"
#include "InventoryComponent.h"
#include "ItemDatabaseComponent.h"
#include "Net/UnrealNetwork.h"
#include "BaseItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Pawn.h"
#include "HordeFunctionLibrary.h"

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

void UEquipmentComponent::DropEquippedItem(EEquipmentSlot Slot)
{
	Server_DropEquippedItem(Slot);
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

	InventoryComp->InventoryList.Items.RemoveAll([](const FItemEntry& Item) {
		return Item.bPendingRemoval;
		});
	InventoryComp->InventoryList.MarkArrayDirty();

	const FItemInstance* ItemInstanceToEquip = DatabaseComp->GetItemInstance(ItemID);

	if (ItemInstanceToEquip)
	{
		UE_LOG(LogInventoryUI, Log, TEXT("Server_EquipItem - SUCCESS, Found item in database. Proceeding with equip."));
		Server_UnequipItem(Slot);

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
		[&Slot](const FEquippedItem& Item) { return Item.Slot == Slot; });

	if (FoundIndex != INDEX_NONE)
	{
		FItemInstance ItemToUnequip_Instance = EquippedList.Items[FoundIndex].Item;

		FIntPoint ItemSize = InventoryComp->GetItemSize(ItemToUnequip_Instance.StaticDataID);
		if (InventoryComp->FindFirstEmptySlotForSize(ItemSize).X != -1)
		{
			EquippedList.Items.RemoveAt(FoundIndex);
			EquippedList.MarkArrayDirty();

			FItemEntry EntryToReadd;
			EntryToReadd.UniqueID = ItemToUnequip_Instance.InstanceID;
			EntryToReadd.StaticDataID = ItemToUnequip_Instance.StaticDataID;
			InventoryComp->AddItemFromEntry(EntryToReadd, 1, 100);

			HandleUnequip(Slot, ItemToUnequip_Instance);
			OnEquipmentChanged.Broadcast(Slot, FItemInstance());
		}
	}
}

void UEquipmentComponent::Server_DropEquippedItem_Implementation(EEquipmentSlot Slot)
{
	AGASPlayerState* PlayerState = GetOwner<AGASPlayerState>();
	if (!PlayerState || !PlayerState->HasAuthority()) return;

	const int32 FoundIndex = EquippedList.Items.IndexOfByPredicate(
		[&Slot](const FEquippedItem& Item) { return Item.Slot == Slot; });

	if (FoundIndex != INDEX_NONE)
	{
		const FItemInstance ItemToDrop_Instance = EquippedList.Items[FoundIndex].Item;

		EquippedList.Items.RemoveAt(FoundIndex);
		EquippedList.MarkArrayDirty();

		if (UItemDatabaseComponent* DatabaseComp = PlayerState->GetItemDatabaseComponent())
		{
			DatabaseComp->RemoveItem(ItemToDrop_Instance.InstanceID);
		}

		// --- MODIFIED SECTION: This logic now directly accesses the C++ property ---
		if (UAssetManager* AM = UAssetManager::GetIfInitialized())
		{
			if (UBaseItemDataAsset* DA = Cast<UBaseItemDataAsset>(AM->GetPrimaryAssetObject(ItemToDrop_Instance.StaticDataID)))
			{
				if (TSubclassOf<AActor> PickupClass = DA->PickupActorClass.LoadSynchronous())
				{
					if (APawn* Pawn = PlayerState->GetPawn())
					{
						FVector SpawnLocation = Pawn->GetActorLocation() + (Pawn->GetActorForwardVector() * 200.0f);
						FRotator SpawnRotation = Pawn->GetActorRotation();
						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = Pawn;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

						if (AActor* NewPickup = GetWorld()->SpawnActor<AActor>(PickupClass, SpawnLocation, SpawnRotation, SpawnParams))
						{
							UHordeFunctionLibrary::InitializePickupActor(NewPickup, ItemToDrop_Instance);
							if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(NewPickup->GetRootComponent()))
							{
								PrimComp->SetSimulatePhysics(true);
							}
						}
					}
				}
			}
		}

		HandleUnequip(Slot, ItemToDrop_Instance);
		OnEquipmentChanged.Broadcast(Slot, FItemInstance());
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
		ClearAllHandlesForSlot(Slot);
		OnItemEquipped(Slot, ItemInstance);
	}
}

void UEquipmentComponent::HandleUnequip(EEquipmentSlot Slot, const FItemInstance& ItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		OnItemUnequipped(Slot, ItemInstance);
		ClearAllHandlesForSlot(Slot);
	}
}