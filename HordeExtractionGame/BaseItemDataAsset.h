// BaseItemDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemTypes.h"
#include "EquipmentSystemTypes.h"
#include "LatentActions.h"
#include "BaseItemDataAsset.generated.h"

class UGameplayEffect;
class UTexture2D;
class USkeletalMesh;
class UPhysicsAsset;
class UAnimMontage; // <-- Added for Anim Montages
class USoundCue;   // <-- Added for Sound Cues



// --- C++ Base Struct for FWeaponAnimationSet ---
// This now fully defines the struct.
USTRUCT(BlueprintType)
struct FWeaponAnimationSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> Character1P_Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> Character3P_Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> Weapon_Montage;
};

// --- C++ Base Struct for FWeaponSoundSet ---
// This now fully defines the struct.
USTRUCT(BlueprintType)
struct FWeaponSoundSet
{
	GENERATED_BODY()

	// Assuming your sound struct has variables like this.
	// Please adjust these to match your actual FWeaponSoundSet Blueprint struct.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TSoftObjectPtr<USoundCue> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TSoftObjectPtr<USoundCue> ReloadSound;
};


UCLASS(BlueprintType, Blueprintable)
class HORDEEXTRACTIONGAME_API UBaseItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Description")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText WeaponDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TSoftObjectPtr<USkeletalMesh> WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float MinBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float MaxBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float MinFireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float MaxFireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSoftClassPtr<UGameplayEffect>> GrantedEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<FAbilityGrantInfo> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnimationSet FireAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnimationSet ReloadAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnimationSet ADSFireAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnimationSet EquipAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	int32 MinMagazineAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	int32 MaxMagazineAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FWeaponSoundSet Sounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<EEquipmentSlot> ValidSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FIntPoint GridSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSoftClassPtr<AActor> WeaponBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World")
	TSoftClassPtr<AActor> PickupActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics")
	TSoftObjectPtr<UPhysicsAsset> PhysicsAsset;
};