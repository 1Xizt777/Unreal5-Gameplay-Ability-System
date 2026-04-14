// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{  
	//引擎自带委托 ， 这个委托不是用来算账的 ，它是用来做“视觉和消息反馈”的最高优先级入口
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);

}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, 
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	
	
	//从Spec里面拿出AllAssetTags装进TagContainer
	EffectSpec.GetAllAssetTags(TagContainer);
	
	
	//绑定广播，并传递TagContainer   在OverlayWidgetController监听
	EffectAssetTags.Broadcast(TagContainer);
}
