// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgertController/OverlayWidgetController.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	//为了让血条一出来就是满的（而不是空血），必须手动广播一次初始值
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());   //硬核宏直接传float的GetHealth()
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}


//GAS底层广播监听数值变化
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	
							//属性值改变委托  
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{	//将Data拆分，只传float的NewValue给蓝图更新UI
			OnHealthChanged.Broadcast(Data.NewValue);
		}	
	);
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
														//寻找"Message"Tag
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(TEXT("Message"));
				
					//如果比对到其以及其子类返回true
				if (Tag.MatchesTag(MessageTag))
				{	
					//拿着Message.HealthPotion去找对应的Row（图标图片，文字提示等）
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					
					//传入ROW给蓝图做UI
					MessageWidgetDataDelegate.Broadcast(*Row);
				}
	
			}
		}
	);
	
}





