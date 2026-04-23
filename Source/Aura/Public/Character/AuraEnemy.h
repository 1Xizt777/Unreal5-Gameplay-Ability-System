// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgertController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"


class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:

	AAuraEnemy();
	
	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* end Enemy Interface */
	
	/* Combat Interface*/
	virtual int32 GetPlayerLevel() override;
	/* end Combat Interface*/
	
	virtual void Die() override;
	
	UPROPERTY(EditAnywhere,Category="LifeSpan")
	float Lifespan = 5.f;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnMaxHealthChanged;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float BaseWalkSpeed = 300.f;
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;
	
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>  HealthBar;
	
	void HitReactCallBack(const FGameplayTag CallBackTag, int32 Counts);
	
};
