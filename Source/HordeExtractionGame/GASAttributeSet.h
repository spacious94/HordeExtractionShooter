// GASAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASAttributeSet.generated.h"

// This macro is a huge time-saver for creating getter and setter functions for your attributes.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HORDEEXTRACTIONGAME_API UGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGASAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --- Primary Vitals & Resources ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxShield);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Vitals", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resources", ReplicatedUsing = OnRep_ReserveAmmo)
	FGameplayAttributeData ReserveAmmo;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ReserveAmmo);

	// --- Core RPG Stats ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Core Stats", ReplicatedUsing = OnRep_Toughness)
	FGameplayAttributeData Toughness; // General damage reduction
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Toughness);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Core Stats", ReplicatedUsing = OnRep_Firepower)
	FGameplayAttributeData Firepower; // General damage multiplier
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Firepower);

	// --- Player Movement ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Movement", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Movement", ReplicatedUsing = OnRep_JumpHeightBonus)
	FGameplayAttributeData JumpHeightBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, JumpHeightBonus);

	// --- Offensive Firearm Stats ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_FireRateMultiplier)
	FGameplayAttributeData FireRateMultiplier;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FireRateMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_CritChance)
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_CritBonusMultiplier)
	FGameplayAttributeData CritBonusMultiplier;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, CritBonusMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_HeadshotDamageMultiplier)
	FGameplayAttributeData HeadshotDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, HeadshotDamageMultiplier);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Firearm Offense", ReplicatedUsing = OnRep_WeakpointDamageMultiplier)
	FGameplayAttributeData WeakpointDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, WeakpointDamageMultiplier);

	// --- Player Handling & Utility ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Handling", ReplicatedUsing = OnRep_ReloadSpeed)
	FGameplayAttributeData ReloadSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ReloadSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Handling", ReplicatedUsing = OnRep_ADSSpeed)
	FGameplayAttributeData ADSSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ADSSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Handling", ReplicatedUsing = OnRep_WeaponSwapSpeed)
	FGameplayAttributeData WeaponSwapSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, WeaponSwapSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Utility", ReplicatedUsing = OnRep_Lifesteal)
	FGameplayAttributeData Lifesteal;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Lifesteal);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Utility", ReplicatedUsing = OnRep_CooldownReduction)
	FGameplayAttributeData CooldownReduction;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, CooldownReduction);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Utility", ReplicatedUsing = OnRep_InteractionSpeed)
	FGameplayAttributeData InteractionSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, InteractionSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Utility", ReplicatedUsing = OnRep_ItemRarity)
	FGameplayAttributeData ItemRarity;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ItemRarity);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Utility", ReplicatedUsing = OnRep_VisionBonus)
	FGameplayAttributeData VisionBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, VisionBonus);

	// --- Defensive Stats ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense", ReplicatedUsing = OnRep_SpellResist)
	FGameplayAttributeData SpellResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, SpellResist);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Defense", ReplicatedUsing = OnRep_Tenacity)
	FGameplayAttributeData Tenacity;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Tenacity);

	// --- Resistances ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resistances", ReplicatedUsing = OnRep_FireResist)
	FGameplayAttributeData FireResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FireResist);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resistances", ReplicatedUsing = OnRep_FrostResist)
	FGameplayAttributeData FrostResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FrostResist);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resistances", ReplicatedUsing = OnRep_ShadowResist)
	FGameplayAttributeData ShadowResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ShadowResist);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resistances", ReplicatedUsing = OnRep_PoisonResist)
	FGameplayAttributeData PoisonResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, PoisonResist);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Resistances", ReplicatedUsing = OnRep_AllResist)
	FGameplayAttributeData AllResist;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, AllResist);

	// --- Damage Type Bonuses ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Damage Bonus", ReplicatedUsing = OnRep_PhysicalDamageBonus)
	FGameplayAttributeData PhysicalDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, PhysicalDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Damage Bonus", ReplicatedUsing = OnRep_FireDamageBonus)
	FGameplayAttributeData FireDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FireDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Damage Bonus", ReplicatedUsing = OnRep_FrostDamageBonus)
	FGameplayAttributeData FrostDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FrostDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Damage Bonus", ReplicatedUsing = OnRep_ShadowDamageBonus)
	FGameplayAttributeData ShadowDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ShadowDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Damage Bonus", ReplicatedUsing = OnRep_PoisonDamageBonus)
	FGameplayAttributeData PoisonDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, PoisonDamageBonus);

	// --- Weapon Type Bonuses ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_RifleDamageBonus)
	FGameplayAttributeData RifleDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, RifleDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_PistolDamageBonus)
	FGameplayAttributeData PistolDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, PistolDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_ShotgunDamageBonus)
	FGameplayAttributeData ShotgunDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ShotgunDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_BowDamageBonus)
	FGameplayAttributeData BowDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, BowDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_SniperRifleDamageBonus)
	FGameplayAttributeData SniperRifleDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, SniperRifleDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_MeleeDamageBonus)
	FGameplayAttributeData MeleeDamageBonus;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MeleeDamageBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Weapon Bonus", ReplicatedUsing = OnRep_MeleeSpeed)
	FGameplayAttributeData MeleeSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MeleeSpeed);

	// --- Enemy Only Stats ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Enemy", ReplicatedUsing = OnRep_BaseMeleeDamage)
	FGameplayAttributeData BaseMeleeDamage;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, BaseMeleeDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Enemy", ReplicatedUsing = OnRep_ProjectileDamage)
	FGameplayAttributeData ProjectileDamage;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ProjectileDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Enemy", ReplicatedUsing = OnRep_EnemyAttackSpeed)
	FGameplayAttributeData EnemyAttackSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, EnemyAttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Enemy", ReplicatedUsing = OnRep_AggroRadius)
	FGameplayAttributeData AggroRadius;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, AggroRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes | Enemy", ReplicatedUsing = OnRep_ThreatLevel)
	FGameplayAttributeData ThreatLevel;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, ThreatLevel);


protected:
	// --- OnRep Functions ---
	UFUNCTION() virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION() virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION() virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);
	UFUNCTION() virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);
	UFUNCTION() virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION() virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	UFUNCTION() virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION() virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION() virtual void OnRep_ReserveAmmo(const FGameplayAttributeData& OldReserveAmmo);
	UFUNCTION() virtual void OnRep_Toughness(const FGameplayAttributeData& OldToughness);
	UFUNCTION() virtual void OnRep_Firepower(const FGameplayAttributeData& OldFirepower);
	UFUNCTION() virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
	UFUNCTION() virtual void OnRep_JumpHeightBonus(const FGameplayAttributeData& OldJumpHeightBonus);
	UFUNCTION() virtual void OnRep_FireRateMultiplier(const FGameplayAttributeData& OldFireRateMultiplier);
	UFUNCTION() virtual void OnRep_CritChance(const FGameplayAttributeData& OldCritChance);
	UFUNCTION() virtual void OnRep_CritBonusMultiplier(const FGameplayAttributeData& OldCritBonusMultiplier);
	UFUNCTION() virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	UFUNCTION() virtual void OnRep_HeadshotDamageMultiplier(const FGameplayAttributeData& OldHeadshotDamageMultiplier);
	UFUNCTION() virtual void OnRep_WeakpointDamageMultiplier(const FGameplayAttributeData& OldWeakpointDamageMultiplier);
	UFUNCTION() virtual void OnRep_ReloadSpeed(const FGameplayAttributeData& OldReloadSpeed);
	UFUNCTION() virtual void OnRep_ADSSpeed(const FGameplayAttributeData& OldADSSpeed);
	UFUNCTION() virtual void OnRep_WeaponSwapSpeed(const FGameplayAttributeData& OldWeaponSwapSpeed);
	UFUNCTION() virtual void OnRep_Lifesteal(const FGameplayAttributeData& OldLifesteal);
	UFUNCTION() virtual void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction);
	UFUNCTION() virtual void OnRep_InteractionSpeed(const FGameplayAttributeData& OldInteractionSpeed);
	UFUNCTION() virtual void OnRep_ItemRarity(const FGameplayAttributeData& OldItemRarity);
	UFUNCTION() virtual void OnRep_VisionBonus(const FGameplayAttributeData& OldVisionBonus);
	UFUNCTION() virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION() virtual void OnRep_SpellResist(const FGameplayAttributeData& OldSpellResist);
	UFUNCTION() virtual void OnRep_Tenacity(const FGameplayAttributeData& OldTenacity);
	UFUNCTION() virtual void OnRep_FireResist(const FGameplayAttributeData& OldFireResist);
	UFUNCTION() virtual void OnRep_FrostResist(const FGameplayAttributeData& OldFrostResist);
	UFUNCTION() virtual void OnRep_ShadowResist(const FGameplayAttributeData& OldShadowResist);
	UFUNCTION() virtual void OnRep_PoisonResist(const FGameplayAttributeData& OldPoisonResist);
	UFUNCTION() virtual void OnRep_AllResist(const FGameplayAttributeData& OldAllResist);
	UFUNCTION() virtual void OnRep_PhysicalDamageBonus(const FGameplayAttributeData& OldPhysicalDamageBonus);
	UFUNCTION() virtual void OnRep_FireDamageBonus(const FGameplayAttributeData& OldFireDamageBonus);
	UFUNCTION() virtual void OnRep_FrostDamageBonus(const FGameplayAttributeData& OldFrostDamageBonus);
	UFUNCTION() virtual void OnRep_ShadowDamageBonus(const FGameplayAttributeData& OldShadowDamageBonus);
	UFUNCTION() virtual void OnRep_PoisonDamageBonus(const FGameplayAttributeData& OldPoisonDamageBonus);
	UFUNCTION() virtual void OnRep_RifleDamageBonus(const FGameplayAttributeData& OldRifleDamageBonus);
	UFUNCTION() virtual void OnRep_PistolDamageBonus(const FGameplayAttributeData& OldPistolDamageBonus);
	UFUNCTION() virtual void OnRep_ShotgunDamageBonus(const FGameplayAttributeData& OldShotgunDamageBonus);
	UFUNCTION() virtual void OnRep_BowDamageBonus(const FGameplayAttributeData& OldBowDamageBonus);
	UFUNCTION() virtual void OnRep_SniperRifleDamageBonus(const FGameplayAttributeData& OldSniperRifleDamageBonus);
	UFUNCTION() virtual void OnRep_MeleeDamageBonus(const FGameplayAttributeData& OldMeleeDamageBonus);
	UFUNCTION() virtual void OnRep_MeleeSpeed(const FGameplayAttributeData& OldMeleeSpeed);
	UFUNCTION() virtual void OnRep_BaseMeleeDamage(const FGameplayAttributeData& OldBaseMeleeDamage);
	UFUNCTION() virtual void OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage);
	UFUNCTION() virtual void OnRep_EnemyAttackSpeed(const FGameplayAttributeData& OldEnemyAttackSpeed);
	UFUNCTION() virtual void OnRep_AggroRadius(const FGameplayAttributeData& OldAggroRadius);
	UFUNCTION() virtual void OnRep_ThreatLevel(const FGameplayAttributeData& OldThreatLevel);
};
