

#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
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
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitAbilityActorInfo();	
	
	UAuraAbilitySystemLibrary::InitializedCommonAbilities(this,AbilitySystemComponent);
	
	
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this); //此函数会调用	WidgetControllerSet()蓝图实现函数
	}
	
	if (UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda
		(
		[this](const FOnAttributeChangeData& Data)
			{
			OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
        	
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda
		(
		[this](const FOnAttributeChangeData& Data)
			{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		FAuraGameplayTags GameplayTags = FAuraGameplayTags().Get();
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,&AAuraEnemy::HitReactCallBack);
		
		//手动发送初始数据（让 UI 填满血条）
		OnHealthChanged.Broadcast(AuraAS->GetMaxHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
	
}

void AAuraEnemy::HitReactCallBack(const FGameplayTag CallBackTag, int32 Counts)
{
	bHitReacting = Counts > 0 ;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting? 0 : BaseWalkSpeed;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	//初始化信息，告诉ASC“AS是谁的”以及“肉体在哪里
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	InitializeDefaultAttributes();  //初始化三大属性
	
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializedDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
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

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(Lifespan);
	Super::Die();
}

