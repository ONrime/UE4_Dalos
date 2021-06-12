// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Weapone/Public/Rifle/AK_Weapone.h"

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
	SetWeaponeLever(WEAPONLEVER::SINGLEFIRE);
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
