// ItemTypes.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/PrimaryAssetId.h"
#include "Abilities/GameplayAbility.h"
#include "GASInputID.h"
#include "ItemTypes.generated.h"

// Forward declaration
class UPrimaryDataAsset;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common		UMETA(DisplayName = "Common"),
	Uncommon	UMETA(DisplayName = "Uncommon"),
	Rare		UMETA(DisplayName = "Rare"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DisplayName = "Legendary"),
	Mythic		UMETA(DisplayName = "Mythic")
};

USTRUCT(BlueprintType)
struct FItemAffix
{
	GENERATED_BODY()

	// Currently empty, but ready for future implementation.
};

USTRUCT(BlueprintType)
struct FAbilityGrantInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> AbilityClass;

	// If None, the ability will try to get the InputID from its interface.
	// Use this to override the default on a per-item basis if needed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityInputID InputID = EAbilityInputID::None;
};


USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	FGuid InstanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	FPrimaryAssetId StaticDataID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	int32 MaxMagazineAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	float RolledDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	float RolledFireRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Instance")
	TArray<FItemAffix> Affixes;

	FItemInstance()
	{
		InstanceID = FGuid::NewGuid();
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FItemInstance> : public TStructOpsTypeTraitsBase2<FItemInstance>
{
	enum
	{
		WithNetSerializer = true,
	};
};
