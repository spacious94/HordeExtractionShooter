// GASEnemyCharacter.cpp
#include "GASEnemyCharacter.h"
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"

AGASEnemyCharacter::AGASEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the Ability System Component directly on the character.
    AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // Minimal is best for AI.

    // Create the Attribute Set.
    AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");
}

void AGASEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the ASC on BeginPlay for enemies.
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

UAbilitySystemComponent* AGASEnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}