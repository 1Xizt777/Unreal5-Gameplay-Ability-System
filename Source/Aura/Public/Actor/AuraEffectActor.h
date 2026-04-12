
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;


UENUM()
enum class EEffectApplicationPolicy: uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy: uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};



UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;
	
	//实现函数
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor , TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	
	bool bDestoryOnEffectRemoval = false;
	
	
	//GE类
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	
	
	
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;
	
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect")
	float ActorLevel = 1.f;
};
