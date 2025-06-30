// GASAttributeSet.cpp

#include "GASAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGASAttributeSet::UGASAttributeSet()
{
	// --- Initialize Vitals & Resources ---
	Health.SetBaseValue(100.0f); Health.SetCurrentValue(100.0f);
	MaxHealth.SetBaseValue(100.0f); MaxHealth.SetCurrentValue(100.0f);
	Shield.SetBaseValue(0.0f); Shield.SetCurrentValue(0.0f);
	MaxShield.SetBaseValue(0.0f); MaxShield.SetCurrentValue(0.0f);
	Stamina.SetBaseValue(100.0f); Stamina.SetCurrentValue(100.0f);
	MaxStamina.SetBaseValue(100.0f); MaxStamina.SetCurrentValue(100.0f);
	Mana.SetBaseValue(100.0f); Mana.SetCurrentValue(100.0f);
	MaxMana.SetBaseValue(100.0f); MaxMana.SetCurrentValue(100.0f);
	ReserveAmmo.SetBaseValue(60.0f); ReserveAmmo.SetCurrentValue(60.0f);

	// --- Initialize Core RPG Stats (Percentages are 0, Multipliers are 1 by default) ---
	Toughness.SetBaseValue(0.0f); Toughness.SetCurrentValue(0.0f);
	Firepower.SetBaseValue(1.0f); Firepower.SetCurrentValue(1.0f);

	// --- Initialize Player Movement ---
	MovementSpeed.SetBaseValue(300.0f); MovementSpeed.SetCurrentValue(300.0f);
	JumpHeightBonus.SetBaseValue(0.0f); JumpHeightBonus.SetCurrentValue(0.0f);

	// --- Initialize Offensive Firearm Stats ---
	FireRateMultiplier.SetBaseValue(1.0f); FireRateMultiplier.SetCurrentValue(1.0f);
	CritChance.SetBaseValue(0.05f); CritChance.SetCurrentValue(0.05f); // 5% base crit
	CritBonusMultiplier.SetBaseValue(1.5f); CritBonusMultiplier.SetCurrentValue(1.5f); // 150% crit damage
	ArmorPenetration.SetBaseValue(0.0f); ArmorPenetration.SetCurrentValue(0.0f);
	HeadshotDamageMultiplier.SetBaseValue(2.0f); HeadshotDamageMultiplier.SetCurrentValue(2.0f);
	WeakpointDamageMultiplier.SetBaseValue(1.5f); WeakpointDamageMultiplier.SetCurrentValue(1.5f);

	// --- Initialize Player Handling & Utility ---
	ReloadSpeed.SetBaseValue(1.0f); ReloadSpeed.SetCurrentValue(1.0f);
	ADSSpeed.SetBaseValue(1.0f); ADSSpeed.SetCurrentValue(1.0f);
	WeaponSwapSpeed.SetBaseValue(1.0f); WeaponSwapSpeed.SetCurrentValue(1.0f);
	Lifesteal.SetBaseValue(0.0f); Lifesteal.SetCurrentValue(0.0f);
	CooldownReduction.SetBaseValue(0.0f); CooldownReduction.SetCurrentValue(0.0f);
	InteractionSpeed.SetBaseValue(1.0f); InteractionSpeed.SetCurrentValue(1.0f);
	ItemRarity.SetBaseValue(0.0f); ItemRarity.SetCurrentValue(0.0f);
	VisionBonus.SetBaseValue(0.0f); VisionBonus.SetCurrentValue(0.0f);

	// --- Initialize Defensive Stats ---
	Armor.SetBaseValue(0.0f); Armor.SetCurrentValue(0.0f);
	SpellResist.SetBaseValue(0.0f); SpellResist.SetCurrentValue(0.0f);
	Tenacity.SetBaseValue(0.0f); Tenacity.SetCurrentValue(0.0f);

	// --- Initialize Resistances ---
	FireResist.SetBaseValue(0.0f); FireResist.SetCurrentValue(0.0f);
	FrostResist.SetBaseValue(0.0f); FrostResist.SetCurrentValue(0.0f);
	ShadowResist.SetBaseValue(0.0f); ShadowResist.SetCurrentValue(0.0f);
	PoisonResist.SetBaseValue(0.0f); PoisonResist.SetCurrentValue(0.0f);
	AllResist.SetBaseValue(0.0f); AllResist.SetCurrentValue(0.0f);

	// --- Initialize Damage Type Bonuses ---
	PhysicalDamageBonus.SetBaseValue(0.0f); PhysicalDamageBonus.SetCurrentValue(0.0f);
	FireDamageBonus.SetBaseValue(0.0f); FireDamageBonus.SetCurrentValue(0.0f);
	FrostDamageBonus.SetBaseValue(0.0f); FrostDamageBonus.SetCurrentValue(0.0f);
	ShadowDamageBonus.SetBaseValue(0.0f); ShadowDamageBonus.SetCurrentValue(0.0f);
	PoisonDamageBonus.SetBaseValue(0.0f); PoisonDamageBonus.SetCurrentValue(0.0f);

	// --- Initialize Weapon Type Bonuses ---
	RifleDamageBonus.SetBaseValue(0.0f); RifleDamageBonus.SetCurrentValue(0.0f);
	PistolDamageBonus.SetBaseValue(0.0f); PistolDamageBonus.SetCurrentValue(0.0f);
	ShotgunDamageBonus.SetBaseValue(0.0f); ShotgunDamageBonus.SetCurrentValue(0.0f);
	BowDamageBonus.SetBaseValue(0.0f); BowDamageBonus.SetCurrentValue(0.0f);
	SniperRifleDamageBonus.SetBaseValue(0.0f); SniperRifleDamageBonus.SetCurrentValue(0.0f);
	MeleeDamageBonus.SetBaseValue(0.0f); MeleeDamageBonus.SetCurrentValue(0.0f);
	MeleeSpeed.SetBaseValue(1.0f); MeleeSpeed.SetCurrentValue(1.0f);

	// --- Initialize Enemy Only Stats ---
	BaseMeleeDamage.SetBaseValue(10.0f); BaseMeleeDamage.SetCurrentValue(10.0f);
	ProjectileDamage.SetBaseValue(10.0f); ProjectileDamage.SetCurrentValue(10.0f);
	EnemyAttackSpeed.SetBaseValue(1.0f); EnemyAttackSpeed.SetCurrentValue(1.0f);
	AggroRadius.SetBaseValue(1000.0f); AggroRadius.SetCurrentValue(1000.0f);
	ThreatLevel.SetBaseValue(1.0f); ThreatLevel.SetCurrentValue(1.0f);
}

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Firepower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, JumpHeightBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FireRateMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CritBonusMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, HeadshotDamageMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, WeakpointDamageMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ReloadSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ADSSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, WeaponSwapSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Lifesteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, InteractionSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ItemRarity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, VisionBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, SpellResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Tenacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FireResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FrostResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ShadowResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, PoisonResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, AllResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, PhysicalDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FireDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FrostDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ShadowDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, PoisonDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, RifleDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, PistolDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ShotgunDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, BowDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, SniperRifleDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MeleeDamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MeleeSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, BaseMeleeDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ProjectileDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, EnemyAttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, AggroRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ThreatLevel, COND_None, REPNOTIFY_Always);
}

// --- OnRep Function Implementations ---
void UGASAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Health, OldHealth); }
void UGASAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxHealth, OldMaxHealth); }
void UGASAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Shield, OldShield); }
void UGASAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxShield, OldMaxShield); }
void UGASAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Stamina, OldStamina); }
void UGASAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxStamina, OldMaxStamina); }
void UGASAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Mana, OldMana); }
void UGASAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxMana, OldMaxMana); }
void UGASAttributeSet::OnRep_ReserveAmmo(const FGameplayAttributeData& OldReserveAmmo) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ReserveAmmo, OldReserveAmmo); }
void UGASAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Toughness, OldToughness); }
void UGASAttributeSet::OnRep_Firepower(const FGameplayAttributeData& OldFirepower) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Firepower, OldFirepower); }
void UGASAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MovementSpeed, OldMovementSpeed); }
void UGASAttributeSet::OnRep_JumpHeightBonus(const FGameplayAttributeData& OldJumpHeightBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, JumpHeightBonus, OldJumpHeightBonus); }
void UGASAttributeSet::OnRep_FireRateMultiplier(const FGameplayAttributeData& OldFireRateMultiplier) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, FireRateMultiplier, OldFireRateMultiplier); }
void UGASAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CritChance, OldCritChance); }
void UGASAttributeSet::OnRep_CritBonusMultiplier(const FGameplayAttributeData& OldCritBonusMultiplier) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CritBonusMultiplier, OldCritBonusMultiplier); }
void UGASAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ArmorPenetration, OldArmorPenetration); }
void UGASAttributeSet::OnRep_HeadshotDamageMultiplier(const FGameplayAttributeData& OldHeadshotDamageMultiplier) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, HeadshotDamageMultiplier, OldHeadshotDamageMultiplier); }
void UGASAttributeSet::OnRep_WeakpointDamageMultiplier(const FGameplayAttributeData& OldWeakpointDamageMultiplier) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, WeakpointDamageMultiplier, OldWeakpointDamageMultiplier); }
void UGASAttributeSet::OnRep_ReloadSpeed(const FGameplayAttributeData& OldReloadSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ReloadSpeed, OldReloadSpeed); }
void UGASAttributeSet::OnRep_ADSSpeed(const FGameplayAttributeData& OldADSSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ADSSpeed, OldADSSpeed); }
void UGASAttributeSet::OnRep_WeaponSwapSpeed(const FGameplayAttributeData& OldWeaponSwapSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, WeaponSwapSpeed, OldWeaponSwapSpeed); }
void UGASAttributeSet::OnRep_Lifesteal(const FGameplayAttributeData& OldLifesteal) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Lifesteal, OldLifesteal); }
void UGASAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CooldownReduction, OldCooldownReduction); }
void UGASAttributeSet::OnRep_InteractionSpeed(const FGameplayAttributeData& OldInteractionSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, InteractionSpeed, OldInteractionSpeed); }
void UGASAttributeSet::OnRep_ItemRarity(const FGameplayAttributeData& OldItemRarity) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ItemRarity, OldItemRarity); }
void UGASAttributeSet::OnRep_VisionBonus(const FGameplayAttributeData& OldVisionBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, VisionBonus, OldVisionBonus); }
void UGASAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Armor, OldArmor); }
void UGASAttributeSet::OnRep_SpellResist(const FGameplayAttributeData& OldSpellResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, SpellResist, OldSpellResist); }
void UGASAttributeSet::OnRep_Tenacity(const FGameplayAttributeData& OldTenacity) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Tenacity, OldTenacity); }
void UGASAttributeSet::OnRep_FireResist(const FGameplayAttributeData& OldFireResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, FireResist, OldFireResist); }
void UGASAttributeSet::OnRep_FrostResist(const FGameplayAttributeData& OldFrostResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, FrostResist, OldFrostResist); }
void UGASAttributeSet::OnRep_ShadowResist(const FGameplayAttributeData& OldShadowResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ShadowResist, OldShadowResist); }
void UGASAttributeSet::OnRep_PoisonResist(const FGameplayAttributeData& OldPoisonResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, PoisonResist, OldPoisonResist); }
void UGASAttributeSet::OnRep_AllResist(const FGameplayAttributeData& OldAllResist) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, AllResist, OldAllResist); }
void UGASAttributeSet::OnRep_PhysicalDamageBonus(const FGameplayAttributeData& OldPhysicalDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, PhysicalDamageBonus, OldPhysicalDamageBonus); }
void UGASAttributeSet::OnRep_FireDamageBonus(const FGameplayAttributeData& OldFireDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, FireDamageBonus, OldFireDamageBonus); }
void UGASAttributeSet::OnRep_FrostDamageBonus(const FGameplayAttributeData& OldFrostDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, FrostDamageBonus, OldFrostDamageBonus); }
void UGASAttributeSet::OnRep_ShadowDamageBonus(const FGameplayAttributeData& OldShadowDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ShadowDamageBonus, OldShadowDamageBonus); }
void UGASAttributeSet::OnRep_PoisonDamageBonus(const FGameplayAttributeData& OldPoisonDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, PoisonDamageBonus, OldPoisonDamageBonus); }
void UGASAttributeSet::OnRep_RifleDamageBonus(const FGameplayAttributeData& OldRifleDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, RifleDamageBonus, OldRifleDamageBonus); }
void UGASAttributeSet::OnRep_PistolDamageBonus(const FGameplayAttributeData& OldPistolDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, PistolDamageBonus, OldPistolDamageBonus); }
void UGASAttributeSet::OnRep_ShotgunDamageBonus(const FGameplayAttributeData& OldShotgunDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ShotgunDamageBonus, OldShotgunDamageBonus); }
void UGASAttributeSet::OnRep_BowDamageBonus(const FGameplayAttributeData& OldBowDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, BowDamageBonus, OldBowDamageBonus); }
void UGASAttributeSet::OnRep_SniperRifleDamageBonus(const FGameplayAttributeData& OldSniperRifleDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, SniperRifleDamageBonus, OldSniperRifleDamageBonus); }
void UGASAttributeSet::OnRep_MeleeDamageBonus(const FGameplayAttributeData& OldMeleeDamageBonus) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MeleeDamageBonus, OldMeleeDamageBonus); }
void UGASAttributeSet::OnRep_MeleeSpeed(const FGameplayAttributeData& OldMeleeSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MeleeSpeed, OldMeleeSpeed); }
void UGASAttributeSet::OnRep_BaseMeleeDamage(const FGameplayAttributeData& OldBaseMeleeDamage) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, BaseMeleeDamage, OldBaseMeleeDamage); }
void UGASAttributeSet::OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ProjectileDamage, OldProjectileDamage); }
void UGASAttributeSet::OnRep_EnemyAttackSpeed(const FGameplayAttributeData& OldEnemyAttackSpeed) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, EnemyAttackSpeed, OldEnemyAttackSpeed); }
void UGASAttributeSet::OnRep_AggroRadius(const FGameplayAttributeData& OldAggroRadius) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, AggroRadius, OldAggroRadius); }
void UGASAttributeSet::OnRep_ThreatLevel(const FGameplayAttributeData& OldThreatLevel) { GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, ThreatLevel, OldThreatLevel); }
