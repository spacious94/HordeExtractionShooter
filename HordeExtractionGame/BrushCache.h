// BrushCache.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/PrimaryAssetId.h"
#include "BrushCache.generated.h"

// Forward-declare the Slate brush struct and TSharedPtr template
struct FSlateBrush;
template <typename ObjectType, ESPMode Mode> class TSharedPtr;

UCLASS()
class HORDEEXTRACTIONGAME_API UBrushCache : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Gets the singleton instance of this subsystem.
	static UBrushCache* Get();

	// Main function to get a brush for a given item ID.
	TSharedPtr<FSlateBrush> GetBrush(const FPrimaryAssetId& ItemId);

private:
	// This map is now private and not a UPROPERTY to hide it from the header tool.
	// It will be managed entirely within the .cpp file.
	TMap<FPrimaryAssetId, TSharedPtr<FSlateBrush>> BrushCache;
};
