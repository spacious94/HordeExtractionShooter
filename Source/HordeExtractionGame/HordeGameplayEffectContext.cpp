// HordeGameplayEffectContext.cpp

#include "HordeGameplayEffectContext.h" // Make sure this matches your project's header include path

bool FHordeGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    // This calls the original parent function to serialize all the default data.
    Super::NetSerialize(Ar, Map, bOutSuccess);

    // This is the new, corrected logic for replicating the HitResult.
    bool bHasValidHitResult = HitResult.IsValid();
    Ar << bHasValidHitResult;
    if (bHasValidHitResult)
    {
        if (Ar.IsLoading())
        {
            // If we are loading (receiving), we need to create a new FHitResult object for our pointer to point to.
            if (!HitResult.IsValid())
            {
                HitResult = MakeShared<FHitResult>();
            }
        }
        // Now we can safely serialize into the FHitResult object itself.
        // Note the use of -> to access the object the pointer points to.
        HitResult->NetSerialize(Ar, Map, bOutSuccess);
    }

    return true;
}