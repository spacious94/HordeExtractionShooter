// ItemDatabaseComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemTypes.h"
#include "Blueprint/DragDropOperation.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ItemDatabaseComponent.generated.h"

// Forward declaration
class UItemDatabaseComponent;

// This is the struct that will be replicated in our array.
// It simply wraps the FItemInstance so the FastArraySerializer can handle it.
USTRUCT()
struct FReplicatedItemInstance : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()                         
	FGuid InstanceID;

	UPROPERTY()
	FItemInstance Item;

	// Replication callbacks
	void PreReplicatedRemove(const struct FItemInstanceList& InArraySerializer);
	void PostReplicatedAdd(const struct FItemInstanceList& InArraySerializer);
	void PostReplicatedChange(const struct FItemInstanceList& InArraySerializer);
};

// This is the FastArraySerializer struct itself.
USTRUCT()
struct FItemInstanceList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FReplicatedItemInstance> Items;

	UPROPERTY(Transient, NotReplicated)
	TObjectPtr<UItemDatabaseComponent> OwningComponent = nullptr;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
};

template<>
struct TStructOpsTypeTraits<FItemInstanceList> : public TStructOpsTypeTraitsBase2<FItemInstanceList>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORDEEXTRACTIONGAME_API UItemDatabaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDatabaseComponent();

	UFUNCTION(BlueprintCallable, Category = "Item Database")
	void Server_CreateNewItemInstance(const FItemInstance& ItemInstance);

	// --- Server-Side Functions ---
	void AddItem(const FItemInstance& ItemInstance);
	void RemoveItem(const FGuid& ItemID);
	const FItemInstance* GetItemInstance(const FGuid& ItemID) const;

	// --- Client-Side Functions ---
	// This function will be called by the OnRep logic to rebuild the client's local TMap.
	void RebuildClientDatabase();
	const FItemInstance* GetClientItemInstance(const FGuid& ItemID) const;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// The server's authoritative database. NOT REPLICATED.
	UPROPERTY()
	TMap<FGuid, FItemInstance> ServerDatabase;

	// The replicated array that mirrors the database for clients. REPLICATED OWNER_ONLY.
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedItems)
	FItemInstanceList ReplicatedItems;

	UFUNCTION()
	void OnRep_ReplicatedItems();

	// The client's local, non-authoritative copy of the database for instant lookups. NOT REPLICATED.
	UPROPERTY(Transient)
	TMap<FGuid, FItemInstance> ClientDatabase;
};
