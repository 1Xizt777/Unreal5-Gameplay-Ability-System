// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/SplineComponent.h"
#include "Engine/LocalPlayer.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/WidgetComponent/DamageTextWidgetComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}



void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	
	AutoRun();
	
	
}

//鼠标光标检测
void AAuraPlayerController::CursorTrace()
{

	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);   //返回光标点击内容
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	if (ThisActor == LastActor) {return;}
	
	if (ThisActor)
	{	//高光
		ThisActor->HighlightActor();
	}
	
	if (LastActor)
	{   //取消高光
		LastActor->UnHighlightActor();
	}
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (AuraContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem*  Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			//添加映射上下文
			Subsystem->AddMappingContext(AuraContext,0);
		}
					
		bShowMouseCursor = true;
		DefaultMouseCursor =EMouseCursor::Default;
			
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);    //鼠标光标可以不受限制地移出游戏窗口
		InputModeData.SetHideCursorDuringCapture(false);     //击了游戏画面空白处时，不要隐藏鼠标光标
		SetInputMode(InputModeData);
	}	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started , this , &AAuraPlayerController::PressedShift);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed , this , &AAuraPlayerController::ReleasedShift);
	
	AuraInputComponent->BindAbilityActions(InputDataConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);}




void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	FVector2D InputActionVector = InputActionValue.Get<FVector2D>();
	
	const FRotator Rotation= GetControlRotation();	
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn * ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection * InputActionVector.Y);
		ControlledPawn->AddMovementInput(RightDirection * -InputActionVector.X);
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAuraAbilitySystemLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


//按键按下
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//如果按下的是鼠标左键
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{	
		//判断点到的是不是敌人模型
		bTargeting = ThisActor ? true : false;
		
		//不在自动奔跑
		bAutoRunning = false;   
		
		FollowTime = 0.f;
	}
}

//按键松开
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//如果松开的按键不是鼠标左键
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
		{	
			AuraASC->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	
	if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
	{	
		AuraASC->AbilityInputTagReleased(InputTag);
	}
	
	
	//如果点到的是敌人模型
	if (!bTargeting && !bShiftIsDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{		
			//我现在在 A 点，我想去 B 点，请给我一份避开所有障碍物的路线图
			if (UNavigationPath* NaviPath =  UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();    //先清空全部曲线点
				
				for (const auto& PathPointLoc : NaviPath->PathPoints)
				{
					Spline->AddSplinePoint(PathPointLoc,ESplineCoordinateSpace::World);
				}

				CachedDestination = NaviPath->PathPoints[NaviPath->PathPoints.Num()-1];	//存储数组最后一个元素的Fvector
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

//按住按键
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//如果按住的按键不是鼠标左键
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
		{	
			AuraASC->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	//如果点到的是敌人模型
	if (bTargeting || bShiftIsDown)
	{
		if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
		{	
			AuraASC->AbilityInputTagHeld(InputTag);
		}
	}

	else  	//如果点到的不是敌人的模型
	{
		//按住时间叠加
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		//获取鼠标点击信息
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		} 
		
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector WorldDirection = (CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);  //角色移动到按住的地点
		}
	}
}


void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning){return;}
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter)
{
	if (DamageTextWidgetComponentClass && IsValid(TargetCharacter))
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter,DamageTextWidgetComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage);
	}
}