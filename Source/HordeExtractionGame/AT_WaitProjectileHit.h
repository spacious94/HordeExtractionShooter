// AT_WaitProjectileHit.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitProjectileHit.generated.h"

class ABP_Projectile_Base;

// This defines the "signature" for our output pins. It's a function that takes one parameter: a Hit Result.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileHitDelegate, const FHitResult&, HitResult);

UCLASS()
class HORDEEXTRACTIONGAME_API UAT_WaitProjectileHit : public UAbilityTask
{
	GENERATED_BODY()

public:
	// --- Output Pins ---
	UPROPERTY(BlueprintAssignable)
	FProjectileHitDelegate OnHit;

	UPROPERTY(BlueprintAssignable)
	FProjectileHitDelegate OnTimeout;

	// --- Core Functions ---

	/** This is the main function that creates our new Blueprint node. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait For Projectile Hit", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAT_WaitProjectileHit* WaitForProjectileHit(UGameplayAbility* OwningAbility, AActor* ProjectileToListenTo, float TimeoutDuration = 5.0f);

protected:
	/** This is the function that starts the task's main job (listening for the hit). */
	virtual void Activate() override;

	/** This is the corrected function signature with all five parameters, matching OnComponentHit. */
	UFUNCTION()
	void HandleProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** This function is called when the timeout timer finishes. */
	UFUNCTION()
	void HandleTimeout();

	/** This function cleans up the task when it's finished. */
	virtual void OnDestroy(bool bInOwnerFinished) override;

	// --- Member Variables ---

	/** A pointer to the projectile we are listening to. */
	UPROPERTY()
	TWeakObjectPtr<AActor> Projectile;

	/** A timer handle for our timeout failsafe. */
	FTimerHandle TimeoutTimerHandle;

	/** The duration of the timeout timer. */
	float Timeout;
};
