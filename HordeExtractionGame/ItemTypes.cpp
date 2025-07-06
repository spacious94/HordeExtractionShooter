// ItemTypes.cpp
#include "ItemTypes.h"

bool FItemInstance::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;

	Ar << InstanceID;
	Ar << StaticDataID;
	Ar << Rarity;
	Ar << MaxMagazineAmmo;
	Ar << RolledDamage;
	Ar << RolledFireRate;
	Ar << Affixes;

	return bOutSuccess;
}

