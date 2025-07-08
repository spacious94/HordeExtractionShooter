// AsyncLoadItemAssetsProxy.h
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "BaseItemDataAsset.h"
#include "Engine/StreamableManager.h"
#include "AsyncLoadItemAssetsProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemAssetsLoadEvent);

/**
 * Blueprint node: “Async Fully Load Item Assets”
 * Exec ? Started ? Completed
 */
UCLASS()
class HORDEEXTRACTIONGAME_API UAsyncLoadItemAssetsProxy
	: public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/** Fires immediately after the request is queued (your “Then” pin). */
	UPROPERTY(BlueprintAssignable)
	FOnItemAssetsLoadEvent Started;

	/** Fires once every referenced soft object is fully loaded. */
	UPROPERTY(BlueprintAssignable)
	FOnItemAssetsLoadEvent Completed;

	/** Factory function that appears as the Blueprint node. */
	UFUNCTION(BlueprintCallable,
		meta = (BlueprintInternalUseOnly = "true",
			WorldContext = "WorldContextObject",
			DisplayName = "Async Fully Load Item Assets",
			Category = "Item Data"))
	static UAsyncLoadItemAssetsProxy* AsyncFullyLoadItemAssets(
		UObject* WorldContextObject,
		UBaseItemDataAsset* Item);

	/* UBlueprintAsyncActionBase */
	virtual void Activate() override;
	/* ---- */

private:
	void HandleFinished();

	/** Weak refs so GC can clean up if level changes mid-load */
	TWeakObjectPtr<UObject> WorldContextObject;
	TWeakObjectPtr<UBaseItemDataAsset> Item;

	/** Keep the handle alive while streaming */
	TSharedPtr<FStreamableHandle> StreamHandle;
};
