// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UDamageTextWidgetComponent;
class UAuraInputDataConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
struct FGameplayTag;
class UAuraAbilitySystemComponent;
class USplineComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float Damage , ACharacter* TargetCharacter);

protected:
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	


private:
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	
	void PressedShift(){ bShiftIsDown = true; }
	void ReleasedShift(){ bShiftIsDown = false; }
	bool bShiftIsDown = false;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	
	UPROPERTY()
	TScriptInterface<IEnemyInterface> LastActor;
	
	UPROPERTY()
	TScriptInterface<IEnemyInterface> ThisActor;
	
	FHitResult CursorHit;
	
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UAuraInputDataConfig> InputDataConfig;
	
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	
	UAuraAbilitySystemComponent* GetAuraASC();
	
	
	FVector CachedDestination = FVector::ZeroVector;     //缓存的目的地
	float FollowTime = 0.f;				 //记录你按住鼠标有多久了
	float ShortPressThreshold = 0.5f;	//用于区别是短按还是长按
	bool bAutoRunning = false;			//是否正在自动奔跑
	bool bTargeting = false;			//用于标记鼠标下方有没有敌人或可互动物品

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	void AutoRun();
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetComponentClass;
};
