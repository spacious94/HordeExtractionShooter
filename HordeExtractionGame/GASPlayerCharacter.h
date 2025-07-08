// GASPlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASInputID.h"
#include "InteractionTypes.h" // Added for FInteractionPrompt

#include "GASPlayerCharacter.generated.h"

// This defines the "signature" of our UI update events. 
// It's a function that takes two floats: the new value and the old value.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, float, NewValue, float, OldValue);

// Delegate for when the focused interactable object changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusChangedDelegate, const FInteractionPrompt&, InteractionPrompt);


class UGASAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class HORDEEXTRACTIONGAME_API AGASPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASPlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// --- Event Dispatchers for UI Binding ---
	// This creates the public "announcement systems" that our UI can bind to.
	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnMaxShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities | UI")
	FOnAttributeChangedDelegate OnReserveAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Interaction | UI")
	FOnFocusChangedDelegate OnFocusChanged;

	/** Public accessor for the currently focused actor. */
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetFocusedActor() const;

	/** Immediately clears the focused actor and updates the UI. Called after a successful interaction. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ClearInteractionFocus();

	/** A function the server can call to force-clear this client's UI. */
	UFUNCTION(Client, Reliable)
	void Client_ClearInteractionFocus();

	/** A Blueprint-callable function that the server can execute to tell this character's client to clear its focus. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RequestClientClearFocus();


protected:
	// -- Pointers & Handles --
	TWeakObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;

	// -- Interaction --
	// This variable is now replicated so the server knows what the client is looking at.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Interaction")
	TWeakObjectPtr<AActor> FocusedActor;

	FTimerHandle TimerHandle_Interaction;

	// -- Core Engine Overrides --
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Array of input actions and their corresponding ability input IDs to bind. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TArray<FInputAbilityBinding> InputAbilityBindings;

	// -- C++ Attribute Change Listeners --
	virtual void OnHealthAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnShieldAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnMaxShieldAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnStaminaAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnMaxStaminaAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnManaAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnMaxManaAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnMovementSpeedAttributeChanged_Listen(const FOnAttributeChangeData& Data);
	virtual void OnReserveAmmoAttributeChanged_Listen(const FOnAttributeChangeData& Data);

	// -- Interaction --
	void UpdateInteractionFocus();

	UFUNCTION(Server, Reliable)
	void Server_SetFocusedActor(AActor* NewFocusedActor);

	// -- Blueprint-Callable Event for granting abilities --
	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities | UI")
	void OnAbilitiesInitialized();

	/** Helper function to bind all the attribute delegates */
	void BindAttributeDelegates(UGASAbilitySystemComponent* ASC);
};