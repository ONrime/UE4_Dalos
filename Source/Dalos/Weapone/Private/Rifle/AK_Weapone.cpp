// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Weapone/Public/Rifle/AK_Weapone.h"
#include "Dalos/Projectile/Public/Rifle_ProjectileBase.h"

AAK_Weapone::AAK_Weapone()
{
	PrimaryActorTick.bCanEverTick = false;
	BodyMesh->SetIsReplicated(true);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BodyMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	BodyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//BodyMesh->SetCollisionProfileName(TEXT("Item"));

	/*static ConstructorHelpers::FClassFinder<UAnimInstance>GUN_ANIM(TEXT("AnimBlueprint'/Game/IBWP3DPolicePistol/Meshes/Weapons/ABP_PolicePistol.ABP_PolicePistol_C'"));
	if (GUN_ANIM.Succeeded()) {
		BodyMesh->SetAnimInstanceClass(GUN_ANIM.Class);
	}*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKELETALGUN(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SK_KA74U_X.SK_KA74U_X'"));
	if (SKELETALGUN.Succeeded()) {
		BodyMesh->SetSkeletalMesh(SKELETALGUN.Object);
	}
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE(TEXT("AnimMontage'/Game/IBWP3DPolicePistol/Animations/Weapons/ANIM_Police_Pistol-Fire_Montage1.ANIM_Police_Pistol-Fire_Montage1'"));
	if (FIRE_MONTAGE.Succeeded()) {
		FireMontage = FIRE_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOAD_MONTAGE(TEXT("AnimMontage'/Game/IBWP3DPolicePistol/Animations/Weapons/ANIM_Police_Pistol-Reload_Montage.ANIM_Police_Pistol-Reload_Montage'"));
	if (RELOAD_MONTAGE.Succeeded()) {
		ReloadMontage = RELOAD_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>EMPTY_MONTAGE(TEXT("AnimMontage'/Game/IBWP3DPolicePistol/Animations/Weapons/Pistol_Empty_Montage.Pistol_Empty_Montage'"));
	if (EMPTY_MONTAGE.Succeeded()) {
		EmptyMontage = EMPTY_MONTAGE.Object;
	}/**/
	SetWeaponeState(WEAPONSTATE::DROP);
	SetWeaponeLever(WEAPONLEVER::FULLAUTO);
	/*CrossHairRadius = 1.0f;
	//InAmmo = FMath::FRandRange(2.0f, 5.0f);
	MaxInAmmo = 7.0f;
	InAmmo = MaxInAmmo;*/
}

void AAK_Weapone::BeginPlay()
{
	Super::BeginPlay();
	SetWeaponeState(WEAPONSTATE::DROP);
	StateStart(GetWeaponeState());
}

UClass* AAK_Weapone::GetStaticClass()
{
	return AAK_Weapone::StaticClass();
}

AWeaponeBase* AAK_Weapone::SpawnToHand(AActor* owner, FVector loc, FRotator rot)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnToHand"));
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = owner;
	spawnParameters.Instigator = GetInstigator();

	auto weapone = GetWorld()->SpawnActor<AAK_Weapone>(AAK_Weapone::StaticClass(), loc, rot, spawnParameters);
	return weapone;
}

void AAK_Weapone::ProjectileFire(FVector loc, FRotator rot, FRotator bulletRot)
{
	FActorSpawnParameters spawnParameter;
	spawnParameter.Owner = this;
	spawnParameter.Instigator = GetInstigator();

	auto projectile = GetWorld()->SpawnActor<ARifle_ProjectileBase>(ARifle_ProjectileBase::StaticClass(), loc, rot, spawnParameter);
	if (projectile) {
		projectile->SetProjectileVelocity(8000.0f);
		projectile->ProjectileFire(bulletRot.Vector());
	}
}

float AAK_Weapone::GetFireRecoilPitch()
{
	return FMath::RandRange(-0.2f, -0.5f);
}
float AAK_Weapone::GetFireRecoilYaw()
{
	return FMath::RandRange(-0.2f, 0.2f);
}
