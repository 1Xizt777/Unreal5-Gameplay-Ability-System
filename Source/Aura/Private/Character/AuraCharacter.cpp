

#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"


AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//For Server
	InitAbilityActorInfo();

}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//For Client
	InitAbilityActorInfo();
}

//初始化信息
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState * AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{	
		//初始化信息，告诉ASC“AS是谁的”以及“肉体在哪里
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
		
		//ASC
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		//AS
		AttributeSet = AuraPlayerState->GetAttributeSet();
	}
}
