// AsyncLoadItemAssetsProxy.cpp
#include "AsyncLoadItemAssetsProxy.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UAsyncLoadItemAssetsProxy* UAsyncLoadItemAssetsProxy::AsyncFullyLoadItemAssets(
	UObject* WorldContextObject,
	UBaseItemDataAsset* Item)
{
	auto* Proxy = NewObject<UAsyncLoadItemAssetsProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->Item = Item;
	return Proxy;
}

void UAsyncLoadItemAssetsProxy::Activate()
{
	// Safety checks
	if (!Item.IsValid())
	{
		Completed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// 1) Build the list exactly like your old latent function
	TArray<FSoftObjectPath> AssetsToLoad;
	const UBaseItemDataAsset* DA = Item.Get();

	AssetsToLoad.Add(DA->WeaponSkeletalMesh.ToSoftObjectPath());
	AssetsToLoad.Add(DA->Icon.ToSoftObjectPath());
	AssetsToLoad.Add(DA->WeaponBase.ToSoftObjectPath());
	AssetsToLoad.Add(DA->PickupActorClass.ToSoftObjectPath());
	AssetsToLoad.Add(DA->PhysicsAsset.ToSoftObjectPath());

	AssetsToLoad.Add(DA->FireAnims.Character1P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->FireAnims.Character3P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->FireAnims.Weapon_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ReloadAnims.Character1P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ReloadAnims.Character3P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ReloadAnims.Weapon_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ADSFireAnims.Character1P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ADSFireAnims.Character3P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->ADSFireAnims.Weapon_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->EquipAnims.Character1P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->EquipAnims.Character3P_Montage.ToSoftObjectPath());
	AssetsToLoad.Add(DA->EquipAnims.Weapon_Montage.ToSoftObjectPath());

	AssetsToLoad.Add(DA->Sounds.FireSound.ToSoftObjectPath());
	AssetsToLoad.Add(DA->Sounds.ReloadSound.ToSoftObjectPath());

	AssetsToLoad.RemoveAll([](const FSoftObjectPath& P) { return P.IsNull() || !P.IsValid(); });

	// 2) Immediately broadcast “Started” so the extra exec pin fires
	Started.Broadcast();

	if (AssetsToLoad.Num() == 0)
	{
		Completed.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// 3) Kick off async load
	FStreamableDelegate Done = FStreamableDelegate::CreateUObject(
		this, &UAsyncLoadItemAssetsProxy::HandleFinished);

	StreamHandle = UAssetManager::Get().GetStreamableManager()
		.RequestAsyncLoad(AssetsToLoad, Done);
}

void UAsyncLoadItemAssetsProxy::HandleFinished()
{
	Completed.Broadcast();
	SetReadyToDestroy();          // let GC reclaim the proxy
	StreamHandle.Reset();
}
