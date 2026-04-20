// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"   


class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;  //接IAbilitySystemInterface接口并重写此函数
	UAttributeSet* GetAttributeSet() const {return AttributeSet;};     //用ASC可以获取AS，但也留接口调用
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere,Category="WeaponSocketName")
	FName WeaponSocketName;
	
	virtual FVector GetWeaponSocketLocation() override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	
	virtual void InitAbilityActorInfo();
	
	
	UPROPERTY(EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	
	
	//用GE初始化属性值函数
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	void InitializeDefaultAttributes() const;
	
	
	void AddCharacterAbilities();	

private:
	
	//数组，在编辑器里添加元素
	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};
