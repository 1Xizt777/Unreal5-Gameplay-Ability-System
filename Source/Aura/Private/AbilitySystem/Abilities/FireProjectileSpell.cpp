// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FireProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UFireProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UFireProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetWeaponSocketLocation();
		
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);	
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>
			(
			FireProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
		
	
		
		//Add Effect
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(FireBoltGameplayEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
		
		FAuraGameplayTags GameplayTags = FAuraGameplayTags().Get();
		
									//Damage是FScalableFloat的表格
		const float ScaledDamage = Damage.AsInteger(GetAbilityLevel());	
																		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Combat_Damage , ScaledDamage);

		//Spawn阶段把SpecHandle信息填好传递给AuraProjectile,在Overlap时使用
		Projectile->FireBoltEffectSpecHandle = SpecHandle;
		
		
		
		
		
		Projectile->FinishSpawning(SpawnTransform);
		
	}
	
}
