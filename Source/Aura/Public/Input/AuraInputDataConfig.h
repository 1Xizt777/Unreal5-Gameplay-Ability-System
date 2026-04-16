// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputAction.h" 
#include "AuraInputDataConfig.generated.h"



USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraInputDataConfig : public UDataAsset
{
	GENERATED_BODY()
		
public:
	
	const class UInputAction* FindAbilityInputActionForTag(const FGameplayTag& Tag , bool bLogNotFound = false);
	
	
	//数组，每个元素都是FAuraInputAction (在编辑器里添加)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
