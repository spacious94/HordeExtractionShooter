// HordeFunctionLibrary.cpp

#include "HordeFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Engine/PrimaryAssetLabel.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h" // Added for the new function

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
