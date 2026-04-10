// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgertController/AuraWidgetController.h"
#include "UI/WidgertController/OverlayWidgetController.h"
#include "Blueprint/UserWidget.h"



UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		//NewObject给一个UOverlayWidgetController类指针赋实体
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams); //传入四个重要变量方便监听
		OverlayWidgetController->BindCallbacksToDependencies();  //监听
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}


void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass is null!"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass is null!"));
	
	//把WBP_Overlay创建好
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	
	
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	
	//将AuraCharacter的四个重要变量!!!!打包!!!!//
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	
	//!!!!传值!!!!//
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	
	//AuraUserWidget类认主，基于AuraUserWidget类的WBP_Overlay的EventWidgetControllerSet节点激活
	OverlayWidget->SetWidgetController(WidgetController);
	
	//初始化属性值，避免UI空槽
	WidgetController->BroadcastInitialValues();
	
	
	//把WBP_Overlay打印到屏幕上
	Widget->AddToViewport();
}


