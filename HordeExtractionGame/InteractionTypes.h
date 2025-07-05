// InteractionTypes.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "InteractionTypes.generated.h"

USTRUCT(BlueprintType)
struct FInteractionPrompt
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    FText InteractionText;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    FText ItemName;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    FText ItemDescription;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<UTexture2D> ItemIcon;
};
