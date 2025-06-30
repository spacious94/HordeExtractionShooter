// AT_WaitProjectileHit.cpp

#include "AT_WaitProjectileHit.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"

// This is our main "creator" function. It sets up the new task object.
UAT_WaitProjectileHit* UAT_WaitProjectileHit::WaitForProjectileHit(UGameplayAbility* OwningAbility, AActor* ProjectileToListenTo, float TimeoutDuration)
{
	UAT_WaitProjectileHit* MyTask = NewAbilityTask<UAT_WaitProjectileHit>(OwningAbility);
	MyTask->Projectile = ProjectileToListenTo;
	MyTask->Timeout = TimeoutDuration;
	return MyTask;
}

// This function is called when the task is officially started.
void UAT_WaitProjectileHit::Activate()
{
	Super::Activate();

	if (!Projectile.IsValid())
	{
		HandleTimeout();
		return;
	}

	UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(Projectile->GetRootComponent());
	if (!CollisionComponent)
	{
		HandleTimeout();
		return;
	}

	// This now correctly binds to our new function with the matching signature.
	CollisionComponent->OnComponentHit.AddDynamic(this, &UAT_WaitProjectileHit::HandleProjectileHit);

	if (Timeout > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeoutTimerHandle, this, &UAT_WaitProjectileHit::HandleTimeout, Timeout, false);
	}
}

// This is the corrected function definition. It now accepts all the parameters from OnComponentHit.
void UAT_WaitProjectileHit::HandleProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// Even though we receive all five parameters, we only need to pass the 'Hit' result to our output pin.
		OnHit.Broadcast(Hit);
	}

	EndTask();
}

// This function is called by the TimerManager if our timeout duration is reached.
void UAT_WaitProjectileHit::HandleTimeout()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTimeout.Broadcast(FHitResult()); // Pass an empty HitResult.
	}

	EndTask();
}

// This function handles all the cleanup.
void UAT_WaitProjectileHit::OnDestroy(bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearTimer(TimeoutTimerHandle);

	if (Projectile.IsValid())
	{
		UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(Projectile->GetRootComponent());
		if (CollisionComponent)
		{
			// This now correctly unbinds from our function with the matching signature.
			CollisionComponent->OnComponentHit.RemoveDynamic(this, &UAT_WaitProjectileHit::HandleProjectileHit);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}
