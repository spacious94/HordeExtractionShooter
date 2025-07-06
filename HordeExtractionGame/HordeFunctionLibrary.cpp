// HordeFunctionLibrary.cpp

#include "HordeFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Engine/PrimaryAssetLabel.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "DragDropOps.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryViewModel.h"

FIntPoint UHordeFunctionLibrary::GetItemSize(const FPrimaryAssetId& ItemStaticID)
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

void UHordeFunctionLibrary::InitializePickupActor(AActor* PickupActor, const FItemInstance& ItemInstance)
{
	if (PickupActor && PickupActor->GetClass()->ImplementsInterface(UPickupInitializationInterface::StaticClass()))
	{
		IPickupInitializationInterface::Execute_InitializePickup(PickupActor, ItemInstance);
	}
}

void UHordeFunctionLibrary::InitializeAbilitySystem(UAbilitySystemComponent* ASC, AActor* OwnerActor, AActor* AvatarActor)
{
	if (ASC)
	{
		ASC->InitAbilityActorInfo(OwnerActor, AvatarActor);
	}
}

bool UHordeFunctionLibrary::GetInventoryDragDropPayload(UDragDropOperation* Operation, UInventoryViewModel*& OutViewModel, FGuid& OutItemID)
{
	if (Operation && Operation->Payload)
	{
		if (const FInventoryDragDropOp* InventoryOp = static_cast<const FInventoryDragDropOp*>(Operation->Payload))
		{
			if (InventoryOp->ViewModel.IsValid())
			{
				OutViewModel = InventoryOp->ViewModel.Get();
				OutItemID = InventoryOp->ItemID;
				return true;
			}
		}
	}
	return false;
}

void UHordeFunctionLibrary::SetDragDropSucceeded(UDragDropOperation* Operation, bool bSucceeded)
{
	if (Operation && Operation->Payload)
	{
		if (FInventoryDragDropOp* InventoryOp = static_cast<FInventoryDragDropOp*>(Operation->Payload))
		{
			InventoryOp->bDropSucceeded = bSucceeded;
		}
	}
}
