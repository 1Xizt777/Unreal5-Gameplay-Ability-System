// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

//自定义结构体FWidgetControllerParams
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// 默认构造函数
	FWidgetControllerParams() {}

	// 带参数的构造函数（方便我们一键打包这四个指针）
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	// 1. 玩家控制器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	// 2. 玩家状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	// 3. GAS 管家
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	// 4. 属性账本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);
	
	//本类不做实现，子类重写
	virtual void BroadcastInitialValues();
	
	//本类不做实现，子类重写
	virtual void BindCallbacksToDependencies();
	
protected:
	
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState ;
	
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
