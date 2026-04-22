// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "FireProjectileSpell.generated.h"


class AAuraProjectile;

UCLASS()
class AURA_API UFireProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
	UFUNCTION(BlueprintCallable , Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	
	UPROPERTY(EditDefaultsOnly , Category = "Combat")
	TSubclassOf<AAuraProjectile> FireProjectileClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> FireBoltGameplayEffectClass;

};
