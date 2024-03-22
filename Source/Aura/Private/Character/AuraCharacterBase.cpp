


#include "Character/AuraCharacterBase.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulicastHandleDeath();

}

void AAuraCharacterBase::MulicastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();

}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{

	return AbilitySystemComponent;
}


FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{


}
void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}
void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AuraASC->AddCharacterAbilities(StartupAbilities);

}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(CharacterDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterailInstance = UMaterialInstanceDynamic::Create(CharacterDissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMaterailInstance);
		StartDissolveTimeline(DynamicMaterailInstance);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterailInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMaterailInstance);
		StartWeaponDissolveTimeline(DynamicMaterailInstance);
	}

}



