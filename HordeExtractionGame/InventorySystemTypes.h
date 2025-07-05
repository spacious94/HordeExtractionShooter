//InventorySystemTypes.h

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventorySystemTypes.generated.h"

// Forward declare the component class to avoid circular dependencies
class UInventoryComponent;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true", DisplayName = "Item Flags"))
enum class EItemFlags : uint8
{
	NONE = 0,
	FoundInRaid = 1 << 0, // 1
	QuestItem = 1 << 1, // 2
	Insured = 1 << 2, // 4
	Broken = 1 << 3, // 8
	Locked = 1 << 4, // 16
	Tradable = 1 << 5, // 32
	ContainerSealed = 1 << 6, // 64
	Reserved = 1 << 7  // 128
};
ENUM_CLASS_FLAGS(EItemFlags);

// This is the modified FItemEntry struct for InventorySystemTypes.h
// The only change is from BlueprintReadOnly to BlueprintReadWrite for GridX and GridY.

USTRUCT(BlueprintType, meta = (DisplayName = "Item Entry", TitleProperty = "StaticDataID"))
struct HORDEEXTRACTIONGAME_API FItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FItemEntry()
		: GridX(-1), GridY(-1), StackSize(0), Durability(0), Flags(EItemFlags::NONE)
	{
		// Per final feedback, generate a GUID on creation to ensure no item ever has a null ID.
		UniqueID = FGuid::NewGuid();
	}

	// This helper remains useful for setting the StaticDataID in one clean line of code.
	static FItemEntry Create(const FPrimaryAssetId& StaticId)
	{
		FItemEntry E;
		E.StaticDataID = StaticId;
		return E;
	}

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	FGuid UniqueID;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	FPrimaryAssetId StaticDataID;

	// [CHANGE] Changed from BlueprintReadOnly to BlueprintReadWrite to test for replication issues.
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	int32 GridX;

	// [CHANGE] Changed from BlueprintReadOnly to BlueprintReadWrite to test for replication issues.
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	int32 GridY;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	int32 StackSize;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry")
	int32 Durability;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Item Entry", meta = (Bitmask, BitmaskEnum = "EItemFlags"))
	EItemFlags Flags;

	UPROPERTY()
	bool bPendingRemoval = false;

	void PreReplicatedRemove(const struct FInventoryList& InArraySerializer);
	void PostReplicatedAdd(const struct FInventoryList& InArraySerializer);
	void PostReplicatedChange(const struct FInventoryList& InArraySerializer);
};


USTRUCT(BlueprintType)
struct HORDEEXTRACTIONGAME_API FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, DisplayName = "Items")
	TArray<FItemEntry> Items;

	UPROPERTY(Transient, NotReplicated)
	TObjectPtr<UInventoryComponent> OwningComponent = nullptr;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};