// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UAuraInputDataConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
struct FGameplayTag;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	

	
protected:
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	

private:
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	
	UPROPERTY()
	TScriptInterface<IEnemyInterface> LastActor;
	
	UPROPERTY()
	TScriptInterface<IEnemyInterface> ThisActor;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UAuraInputDataConfig> InputDataConfig;
};
