// EquipmentSystemTypes.h
#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "EquipmentSystemTypes.generated.h"

class UEquipmentComponent; // Forward Declaration

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None			UMETA(DisplayName = "None"),
	PrimaryWeapon	UMETA(DisplayName = "Primary Weapon"),
	SecondaryWeapon	UMETA(DisplayName = "Secondary Weapon"),
	Sidearm			UMETA(DisplayName = "Sidearm"),
	Helmet			UMETA(DisplayName = "Helmet"),
	Chest			UMETA(DisplayName = "Chest"),
	Legs			UMETA(DisplayName = "Legs"),
	Hands			UMETA(DisplayName = "Hands"),
	Feet			UMETA(DisplayName = "Feet"),
	Backpack		UMETA(DisplayName = "Backpack")
};

USTRUCT(BlueprintType)
struct FEquippedItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EEquipmentSlot Slot = EEquipmentSlot::None;

	UPROPERTY()
	FItemInstance Item;

	void PreReplicatedRemove(const struct FEquippedList& InArraySerializer);
	void PostReplicatedAdd(const struct FEquippedList& InArraySerializer);
	void PostReplicatedChange(const struct FEquippedList& InArraySerializer);
};

USTRUCT(BlueprintType)
struct FEquippedList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FEquippedItem> Items;

	UPROPERTY(Transient, NotReplicated)
	TObjectPtr<UEquipmentComponent> OwningComponent = nullptr;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
};

template<>
struct TStructOpsTypeTraits<FEquippedList> : public TStructOpsTypeTraitsBase2<FEquippedList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
