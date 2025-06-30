// GASEnemyCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASPlayerCharacter.h"
#include "GASEnemyCharacter.generated.h"

class UGASAbilitySystemComponent;
class UGASAttributeSet;

UCLASS()
class HORDEEXTRACTIONGAME_API AGASEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGASEnemyCharacter();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<UGASAttributeSet> AttributeSet;
};