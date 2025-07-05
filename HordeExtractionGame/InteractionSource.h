// InteractionSource.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionTypes.h"
#include "InteractionSource.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionSource : public UInterface
{
    GENERATED_BODY()
};

class HORDEEXTRACTIONGAME_API IInteractionSource
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    FInteractionPrompt GetInteractionPrompt();
};
