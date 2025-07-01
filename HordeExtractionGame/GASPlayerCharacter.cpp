// GASPlayerCharacter.cpp

#include "GASPlayerCharacter.h"
#include "GASPlayerState.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "GASInputID.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AGASPlayerCharacter::AGASPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AGASPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AGASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AGASPlayerState* PS = GetPlayerState<AGASPlayerState>())
	{
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindAttributeDelegates(AbilitySystemComponent.Get());
		OnAbilitiesInitialized(); // Grant abilities on the server
	}
}

void AGASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGASPlayerState* PS = GetPlayerState<AGASPlayerState>())
	{
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindAttributeDelegates(AbilitySystemComponent.Get());
	}
}

void AGASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) { return; }

	UGASAbilitySystemComponent* ASC = Cast<UGASAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!ASC) { return; }

	for (const FInputAbilityBinding& Binding : InputAbilityBindings)
	{
		if (Binding.InputAction && Binding.AbilityInputID != EAbilityInputID::None)
		{
			EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Triggered, ASC, &UGASAbilitySystemComponent::AbilityLocalInputPressed, static_cast<int32>(Binding.AbilityInputID));
			EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Completed, ASC, &UGASAbilitySystemComponent::AbilityLocalInputReleased, static_cast<int32>(Binding.AbilityInputID));
		}
	}
}

void AGASPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unbind all delegates when the character is destroyed to prevent memory leaks.
	if (AbilitySystemComponent.IsValid())
	{
		const UGASAttributeSet* AttributeSet = Cast<UGASAttributeSet>(AbilitySystemComponent->GetAttributeSet(UGASAttributeSet::StaticClass()));
		if (AttributeSet)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetShieldAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxShieldAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetReserveAmmoAttribute()).RemoveAll(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// --- C++ Listeners that Broadcast Public Events ---

void AGASPlayerCharacter::OnHealthAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnMaxHealthAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnShieldAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnShieldChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnMaxShieldAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnMaxShieldChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnStaminaAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnStaminaChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnMaxStaminaAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnMaxStaminaChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnManaAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnMaxManaAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue, Data.OldValue); }
void AGASPlayerCharacter::OnReserveAmmoAttributeChanged_Listen(const FOnAttributeChangeData& Data) { OnReserveAmmoChanged.Broadcast(Data.NewValue, Data.OldValue); }

void AGASPlayerCharacter::OnMovementSpeedAttributeChanged_Listen(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

// --- Helper Function ---

void AGASPlayerCharacter::BindAttributeDelegates(UGASAbilitySystemComponent* ASC)
{
	if (!ASC) { return; }

	const UGASAttributeSet* AttributeSet = Cast<UGASAttributeSet>(ASC->GetAttributeSet(UGASAttributeSet::StaticClass()));
	if (!AttributeSet) { return; }

	// Bind listeners to C++ functions
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGASPlayerCharacter::OnHealthAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMaxHealthAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetShieldAttribute()).AddUObject(this, &AGASPlayerCharacter::OnShieldAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxShieldAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMaxShieldAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnStaminaAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMaxStaminaAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnManaAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMaxManaAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMovementSpeedAttributeChanged_Listen);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetReserveAmmoAttribute()).AddUObject(this, &AGASPlayerCharacter::OnReserveAmmoAttributeChanged_Listen);

	// Set initial UI values by broadcasting the delegates once
	OnHealthChanged.Broadcast(AttributeSet->GetHealth(), 0);
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth(), 0);
	OnShieldChanged.Broadcast(AttributeSet->GetShield(), 0);
	OnMaxShieldChanged.Broadcast(AttributeSet->GetMaxShield(), 0);
	OnStaminaChanged.Broadcast(AttributeSet->GetStamina(), 0);
	OnMaxStaminaChanged.Broadcast(AttributeSet->GetMaxStamina(), 0);
	OnManaChanged.Broadcast(AttributeSet->GetMana(), 0);
	OnMaxManaChanged.Broadcast(AttributeSet->GetMaxMana(), 0);
	OnReserveAmmoChanged.Broadcast(AttributeSet->GetReserveAmmo(), 0);

	// Also set the initial movement speed
	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
}
