

#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


AAuraEnemy::AAuraEnemy()
{
	
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	//ASC
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	
	//AS
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	
	//ReplicationMode
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}



void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	//初始化信息，告诉ASC“AS是谁的”以及“肉体在哪里
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}


void AAuraEnemy::HighlightActor()
{
				//允许它渲染到“自定义深度缓冲”->描边效果
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);  //红色
	
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::UnHighlightActor()
{
			   //取消它渲染到“自定义深度缓冲”->取消描边效果
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
