// HordeGameplayEffectContext.h

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "HordeGameplayEffectContext.generated.h"

USTRUCT(BlueprintType)
struct FHordeGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    // This is the important part!
    // We override this function to support replicating the HitResult.
    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

    // These are also required for the engine to know how to work with our custom struct.
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return StaticStruct();
    }

    virtual FHordeGameplayEffectContext* Duplicate() const override
    {
        FHordeGameplayEffectContext* NewContext = new FHordeGameplayEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }
};

// This is a required templated helper function that the engine uses.
template<>
struct TStructOpsTypeTraits<FHordeGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FHordeGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};