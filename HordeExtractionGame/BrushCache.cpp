// BrushCache.cpp
#include "BrushCache.h"
#include "Engine/AssetManager.h"
#include "Engine/GameInstance.h"
#include "Engine/Texture2D.h"
#include "Brushes/SlateImageBrush.h"

#include "Styling/SlateBrush.h"
#include "Templates/SharedPointer.h"

UBrushCache* UBrushCache::Get()
{
	if (GEngine && GEngine->GameViewport)
	{
		if (UGameInstance* GameInstance = GEngine->GameViewport->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UBrushCache>();
		}
	}
	return nullptr;
}

TSharedPtr<FSlateBrush> UBrushCache::GetBrush(const FPrimaryAssetId& ItemId)
{
	// Check if the brush is already in our cache.
	if (TSharedPtr<FSlateBrush>* FoundBrush = BrushCache.Find(ItemId))
	{
		return *FoundBrush;
	}

	// If not found, we need to create it.
	if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		if (UPrimaryDataAsset* DA = Cast<UPrimaryDataAsset>(AssetManager->GetPrimaryAssetObject(ItemId)))
		{
			// Find the "Icon" property on the Data Asset.
			if (FSoftObjectProperty* IconProp = FindFProperty<FSoftObjectProperty>(DA->GetClass(), "Icon"))
			{
				if (UTexture2D* Tex = Cast<UTexture2D>(IconProp->GetPropertyValue_InContainer(DA).LoadSynchronous()))
				{
					// Create the new brush.
					TSharedPtr<FSlateBrush> NewBrush = MakeShareable(new FSlateImageBrush(Tex, FVector2D(Tex->GetSizeX(), Tex->GetSizeY())));
					
					// Add it to the cache.
					BrushCache.Add(ItemId, NewBrush);
					
					return NewBrush;
				}
			}
		}
	}

	// Return a null pointer if we failed to create the brush.
	return nullptr;
}
