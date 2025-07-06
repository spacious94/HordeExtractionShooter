// Copyright Your Name or Company Name. All Rights Reserved.

#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/AssetManager.h"
#include "Engine/PrimaryAssetLabel.h"
#include "UObject/UnrealType.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HordeExtractionGame.h"
#include "GASPlayerState.h"
#include "GameFramework/Pawn.h"
#include "HordeFunctionLibrary.h"
#include "ItemDatabaseComponent.h"

FIntPoint UInventoryComponent::GetItemSize(const FPrimaryAssetId& ItemStaticID) const
{
	if (UAssetManager* AM = UAssetManager::GetIfInitialized())
	{
		if (UPrimaryDataAsset* DA = Cast<UPrimaryDataAsset>(AM->GetPrimaryAssetObject(ItemStaticID)))
		{
			if (FStructProperty* SizeProp = FindFProperty<FStructProperty>(DA->GetClass(), "GridSize"))
			{
				if (SizeProp->Struct == TBaseStructure<FIntPoint>::Get())
				{
					return *SizeProp->ContainerPtrToValuePtr<FIntPoint>(DA);
				}
			}
		}
	}
	return FIntPoint(1, 1); // Default to 1x1 if size not found
}

bool UInventoryComponent::IsSpaceAvailable(FIntPoint TopLeft, FIntPoint Size, const FGuid& IgnoreItemID) const
{
	// Check bounds of the inventory grid
	const int32 GridWidth = 12;
	const int32 GridHeight = 6;
	if (TopLeft.X < 0 || TopLeft.Y < 0 || TopLeft.X + Size.X > GridWidth || TopLeft.Y + Size.Y > GridHeight)
	{
		return false; // Out of bounds
	}

	// Check for overlap with existing items
	for (const FItemEntry& ExistingItem : InventoryList.Items)
	{
		// Also ignore items that are about to be deleted
		if (ExistingItem.bPendingRemoval)
		{
			continue;
		}

		if (ExistingItem.UniqueID == IgnoreItemID)
		{
			continue; // Don't check against the item we're moving
		}

		FIntPoint ExistingItemSize = GetItemSize(ExistingItem.StaticDataID);
		// Check for rectangle intersection
		if (TopLeft.X < ExistingItem.GridX + ExistingItemSize.X &&
			TopLeft.X + Size.X > ExistingItem.GridX &&
			TopLeft.Y < ExistingItem.GridY + ExistingItemSize.Y &&
			TopLeft.Y + Size.Y > ExistingItem.GridY)
		{
			return false; // Overlap detected
		}
	}

	return true; // Space is available
}

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Crucial step: Set the back-pointer so the FInventoryList knows which component owns it.
	InventoryList.OwningComponent = this;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, InventoryList, COND_OwnerOnly);
}

void UInventoryComponent::OnRep_InventoryList()
{
	// This function is called on clients whenever the InventoryList is replicated.
	// We need to ensure our non-replicated 'OwningComponent' pointer is set correctly.
	InventoryList.OwningComponent = this;
}

// Find the first available empty slot in the inventory grid for a given item size.
FIntPoint UInventoryComponent::FindFirstEmptySlotForSize(FIntPoint Size) const
{
	const int32 GridWidth = 12;
	const int32 GridHeight = 6;

	for (int32 Y = 0; Y <= GridHeight - Size.Y; ++Y)
	{
		for (int32 X = 0; X <= GridWidth - Size.X; ++X)
		{
			if (IsSpaceAvailable(FIntPoint(X, Y), Size))
			{
				return FIntPoint(X, Y); // Found a valid top-left corner.
			}
		}
	}

	return FIntPoint(-1, -1); // No empty slot found.
}

void UInventoryComponent::AddItem(const FPrimaryAssetId& ItemStaticID, int32 StackSize, int32 Durability)
{
	if (GetOwner()->HasAuthority())
	{
		// First, perform garbage collection for any items marked for removal
		InventoryList.Items.RemoveAll([](const FItemEntry& Item)
		{
			return Item.bPendingRemoval;
		});
		InventoryList.MarkArrayDirty(); // Important to notify clients of the removals

		FIntPoint ItemSize = GetItemSize(ItemStaticID);
		FIntPoint EmptySlot = FindFirstEmptySlotForSize(ItemSize);
		if (EmptySlot.X == -1)
		{
			// Optional: Add a log message to indicate the inventory is full.
			UE_LOG(LogInventoryUI, Log, TEXT("Inventory is full for item size %dx%d"), ItemSize.X, ItemSize.Y);
			return; // No space left in the inventory.
		}

		FItemEntry NewEntry = FItemEntry::Create(ItemStaticID);
		NewEntry.GridX = EmptySlot.X;
		NewEntry.GridY = EmptySlot.Y;
		NewEntry.StackSize = StackSize;
		NewEntry.Durability = Durability;
		UE_LOG(LogInventoryUI, Log, TEXT("New entry set to empty slots: (%d, %d)"), NewEntry.GridX, NewEntry.GridY);

		InventoryList.Items.Add(NewEntry);

		// Get a reference to the actual item we just added to the array.
		FItemEntry& AddedEntry = InventoryList.Items.Last();

		// Now use this reference for both MarkItemDirty and the broadcast.
		InventoryList.MarkItemDirty(AddedEntry);

		// The replication callbacks that fire this delegate only execute on clients who receive replicated data.
		// The server/host needs to call it manually since it changes the data directly.
		if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
		{
			OnItemAdded.Broadcast(AddedEntry);
		}
	}
}

void UInventoryComponent::AddItemFromEntry(const FItemEntry& ItemEntry, int32 StackSize, int32 Durability)
{
	if (GetOwner()->HasAuthority())
	{
		FIntPoint ItemSize = GetItemSize(ItemEntry.StaticDataID);
		FIntPoint EmptySlot = FindFirstEmptySlotForSize(ItemSize);
		if (EmptySlot.X == -1)
		{
			UE_LOG(LogInventoryUI, Warning, TEXT("Inventory full, cannot add item %s"), *ItemEntry.UniqueID.ToString());
			return;
		}

		FItemEntry NewEntry = ItemEntry; // Copy the GUID and StaticDataID
		NewEntry.GridX = EmptySlot.X;
		NewEntry.GridY = EmptySlot.Y;
		NewEntry.StackSize = StackSize;
		NewEntry.Durability = Durability;

		InventoryList.Items.Add(NewEntry);
		InventoryList.MarkItemDirty(InventoryList.Items.Last());

		if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
		{
			OnItemAdded.Broadcast(InventoryList.Items.Last());
		}
	}
}

void UInventoryComponent::RemoveItemByID(const FGuid& ItemID)
{
	if (GetOwner()->HasAuthority())
	{
		for (FItemEntry& Item : InventoryList.Items)
		{
			if (Item.UniqueID == ItemID)
			{
				Item.bPendingRemoval = true;
				InventoryList.MarkItemDirty(Item);

				// Manually broadcast for the server/host, as replication callbacks won't fire.
				if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
				{
					OnItemChanged.Broadcast(Item);
				}
				return;
			}
		}
	}
}

void UInventoryComponent::Server_MoveItem_Implementation(const FGuid& ItemID, int32 NewX, int32 NewY)
{
	if (GetOwner()->HasAuthority())
	{
		// Also perform garbage collection when an item is moved
		InventoryList.Items.RemoveAll([](const FItemEntry& Item)
		{
			return Item.bPendingRemoval;
		});
		InventoryList.MarkArrayDirty();
	}

	for (int32 i = 0; i < InventoryList.Items.Num(); ++i)
	{
		if (InventoryList.Items[i].UniqueID == ItemID)
		{
			FItemEntry& ItemToMove = InventoryList.Items[i];
			FIntPoint ItemSize = GetItemSize(ItemToMove.StaticDataID);

			if (IsSpaceAvailable(FIntPoint(NewX, NewY), ItemSize, ItemID))
			{
				ItemToMove.GridX = NewX;
				ItemToMove.GridY = NewY;
				UE_LOG(LogInventoryUI, Log, TEXT("Server: %s -> (%d,%d)"), *ItemID.ToString(), ItemToMove.GridX, ItemToMove.GridY);

				InventoryList.MarkItemDirty(ItemToMove);

				if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
				{
					OnItemChanged.Broadcast(ItemToMove);
				}
			}
			else
			{
				UE_LOG(LogInventoryUI, Log, TEXT("Cell already occupied. Rejecting move."));
				// Optional: Add a client RPC to notify the player the move failed.
			}
			return;
		}
	}
}

void UInventoryComponent::Server_RemoveItem_Implementation(const FGuid& ItemID)
{
	for (int32 i = 0; i < InventoryList.Items.Num(); ++i)
	{
		if (InventoryList.Items[i].UniqueID == ItemID)
		{
			InventoryList.Items.RemoveAt(i);
			InventoryList.MarkArrayDirty(); // Mark the whole array dirty since an item was removed.
			
			// Manually broadcast the delegate on the server.
			if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
			{
				OnItemRemoved.Broadcast(ItemID);
			}
			return;
		}
	}
}

void UInventoryComponent::Server_SplitStack_Implementation(const FGuid& ItemID, int32 AmountToSplit)
{
	for (int32 i = 0; i < InventoryList.Items.Num(); ++i)
	{
		if (InventoryList.Items[i].UniqueID == ItemID)
		{
			FItemEntry& OriginalItem = InventoryList.Items[i];

			// Ensure we can't split more than we have.
			if (AmountToSplit >= OriginalItem.StackSize)
			{
				return;
			}

			FIntPoint EmptySlot = FindFirstEmptySlotForSize(FIntPoint(1, 1)); // A split item is always 1x1
			if (EmptySlot.X == -1)
			{
				return; // No space for the new stack.
			}

			// Reduce the original stack size.
			OriginalItem.StackSize -= AmountToSplit;
			InventoryList.MarkItemDirty(OriginalItem);

			// Create the new stack.
			FItemEntry NewStack = FItemEntry::Create(OriginalItem.StaticDataID);
			NewStack.GridX = EmptySlot.X;
			NewStack.GridY = EmptySlot.Y;
			NewStack.StackSize = AmountToSplit;
			NewStack.Durability = OriginalItem.Durability; // Copy durability.
			NewStack.Flags = OriginalItem.Flags; // Copy flags.

			InventoryList.Items.Add(NewStack);
			InventoryList.MarkItemDirty(InventoryList.Items.Last());

			// Manually broadcast the changes on the server.
			if (GetOwner()->GetNetMode() == ENetMode::NM_ListenServer || GetOwner()->GetNetMode() == ENetMode::NM_Standalone)
			{
				OnItemChanged.Broadcast(OriginalItem);
				OnItemAdded.Broadcast(InventoryList.Items.Last());
			}

			return;
		}
	}
}

void UInventoryComponent::Server_EquipItem_Implementation(const FGuid& ItemID)
{
	// TODO: This will be more complex.
	// TODO: Find the item by GUID.
	// TODO: Remove it from the inventory list.
	// TODO: Add it to a separate "Equipment" list/component.
	// TODO: Grant abilities/effects via the Player State's Ability System Component.
}

void UInventoryComponent::Server_DropItem_Implementation(const FGuid& ItemID)
{
	if (GetOwner()->HasAuthority())
	{
		// Perform garbage collection before dropping the item.
		InventoryList.Items.RemoveAll([](const FItemEntry& Item) {
			return Item.bPendingRemoval;
		});
		InventoryList.MarkArrayDirty();

		AGASPlayerState* PlayerState = GetOwner<AGASPlayerState>();
		if (!PlayerState) return;

		UItemDatabaseComponent* DatabaseComp = PlayerState->GetItemDatabaseComponent();
		if (!DatabaseComp) return;

		const FItemInstance* ItemToDrop_Instance = DatabaseComp->GetItemInstance(ItemID);

		if (ItemToDrop_Instance)
		{
			// This is a copy, which is what we want to pass to the spawn function.
			FItemInstance DroppedItemData = *ItemToDrop_Instance;

			// Remove the item from both the inventory and the database.
			RemoveItemByID(ItemID);
			DatabaseComp->RemoveItem(ItemID);
			
			if (UAssetManager* AM = UAssetManager::GetIfInitialized())
			{
				if (UPrimaryDataAsset* DA = Cast<UPrimaryDataAsset>(AM->GetPrimaryAssetObject(DroppedItemData.StaticDataID)))
				{
					if (FObjectProperty* PickupClassProp = FindFProperty<FObjectProperty>(DA->GetClass(), "PickupActorClass"))
					{
						if (UClass* PickupClass = Cast<UClass>(PickupClassProp->GetPropertyValue_InContainer(DA)))
						{
							APawn* Pawn = PlayerState->GetPawn();
							if (!Pawn) return;

							FVector SpawnLocation = Pawn->GetActorLocation() + (Pawn->GetActorForwardVector() * 200.0f); // Spawn 2m in front
							FRotator SpawnRotation = Pawn->GetActorRotation();

							FActorSpawnParameters SpawnParams;
							SpawnParams.Owner = Pawn;
							SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

							if (AActor* NewPickup = GetWorld()->SpawnActor<AActor>(PickupClass, SpawnLocation, SpawnRotation, SpawnParams))
							{
								UHordeFunctionLibrary::InitializePickupActor(NewPickup, DroppedItemData);

								// Enable physics so it drops to the ground
								if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(NewPickup->GetRootComponent()))
								{
									PrimComp->SetSimulatePhysics(true);
								}
							}
						}
					}
				}
			}
		}
	}
}

TArray<FItemEntry> UInventoryComponent::GetAllItems() const
{
	return InventoryList.Items;
}
