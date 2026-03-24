// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (AuraContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem*  Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
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
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

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
