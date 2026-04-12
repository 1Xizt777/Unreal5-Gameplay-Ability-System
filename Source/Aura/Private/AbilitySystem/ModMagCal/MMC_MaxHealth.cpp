// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCal/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{	
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{																
	//GetAggregatedTags()是获取聚合Tags(把这个角色身上所有的 Tags，无论来源是哪里,全部打包)
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	//塞进这个参数，就是为了保证后面抓出来的值是绝对准确的最终值
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	//提出来最终值
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);
	
	//获取PlayerLevel计算
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	//自定义计算并返回值
	Vigor = FMath::Max<float>(Vigor,0.f);
	
	return 80.f + (Vigor * 2.5f) + (PlayerLevel * 10);
}
