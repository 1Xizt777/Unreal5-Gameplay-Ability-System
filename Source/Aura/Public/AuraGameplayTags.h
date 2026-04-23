// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 *                                             ---  此类用于添加NavtiveTag  ---  
 */
struct FAuraGameplayTags
{
	
public:
	// 获取单例的全局访问点
	static const FAuraGameplayTags& Get(){ return GameplayTags;}	
	static void InitializeNativeGameplayTags();
	
	
	// --- Primary Attributes --- //
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Strength;
	
	
	// --- Secondary Attributes --- //
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	FGameplayTag Combat_Damage;
	
	FGameplayTag Effects_HitReact;
	
protected:
	// 私有的静态实例
	static FAuraGameplayTags GameplayTags;

private:
	
};
