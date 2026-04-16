// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputDataConfig.h"

const UInputAction* UAuraInputDataConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for(FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputTag.MatchesTagExact(Tag))
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Tag : %s Not Find in the class : %s"), *Tag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
