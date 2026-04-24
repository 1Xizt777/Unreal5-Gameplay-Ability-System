// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCal/ExecCal_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"


UExecCal_Damage::UExecCal_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}


void UExecCal_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags =  Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags =  Spec.CapturedTargetTags.GetAggregatedTags();
	
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float BaseDamage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags().Get().Combat_Damage);
	
	
	float TargetBlockChance = 0.f;
	
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef , EvaluateParameters ,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	
	const bool bBlocked = FMath::RandRange(1,100) < TargetBlockChance;
	BaseDamage = bBlocked ? BaseDamage/2.f : BaseDamage;  
	
	
	const FGameplayModifierEvaluatedData EvaluateData(UAuraAttributeSet::GetIncomingDamageAttribute() , EGameplayModOp::Override , BaseDamage);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
