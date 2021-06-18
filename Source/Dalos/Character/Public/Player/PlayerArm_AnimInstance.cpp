// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerArm_AnimInstance.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "PlayerBody_AnimInstance.h"
#include "PlayerArm_AnimInstance.h"

UPlayerArm_AnimInstance::UPlayerArm_AnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AK_FIRE1(TEXT("AnimMontage'/Game/Player/Anim/Arm/Armed/AK/Fire/Armed_AK_Fire_Center_Arm_Montage.Armed_AK_Fire_Center_Arm_Montage'"));
	if (AK_FIRE1.Succeeded()) AK_Fire1_Montage = AK_FIRE1.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AK_FIRE2(TEXT("AnimMontage'/Game/Player/Anim/Arm/Armed/AK/Fire/Armed_AK_Fire_Complex_Arm_Montage.Armed_AK_Fire_Complex_Arm_Montage'"));
	if (AK_FIRE2.Succeeded()) AK_Fire2_Montage = AK_FIRE2.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AK_FIRE(TEXT("AnimMontage'/Game/Player/Anim/Arm/Armed/AK/Fire/Armed_AK_Fire_C_Arm_Montage.Armed_AK_Fire_C_Arm_Montage'"));
	if (AK_FIRE.Succeeded()) AK_Fire_Montage = AK_FIRE.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AK_ADS_FIRE(TEXT("AnimMontage'/Game/Player/Anim/Arm/Armed/AK/Fire/Armed_AK_Fire_C_ADS_Arm_Montage.Armed_AK_Fire_C_ADS_Arm_Montage'"));
	if (AK_ADS_FIRE.Succeeded()) AK_ADS_Fire_Montage = AK_ADS_FIRE.Object;
}

void UPlayerArm_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn)) {
		auto player = Cast<AMultiPlayerBase>(pawn);
		playerSpeed = player->GetVelocity().Size();

		upperStateNClass = player->upperStateNClass;

		FVector playerVelocity = player->GetVelocity();
		playerVelocity.Normalize();
		FMatrix RotMatrix = FRotationMatrix(player->GetActorRotation());
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = playerVelocity.GetSafeNormal2D();

		dirForward = FVector::DotProduct(ForwardVector, NormalizedVel);
		dirRight = FVector::DotProduct(RightVector, NormalizedVel);
		if (FMath::Abs(dirForward) > FMath::Abs(dirRight)) {
			inputDir = FMath::Abs(dirForward);
		}
		else { inputDir = FMath::Abs(dirRight); }

		aimDirRight = player->GetAimDirRight();
		aimDirUp = player->GetAimDirUp();
		//UE_LOG(LogTemp, Warning, TEXT("aimDirRight: %f"), aimDirRight);
	}
}

void UPlayerArm_AnimInstance::PlayFireMontage()
{
	/*fireRandNum = FMath::RandRange(1.0f, 2.0f);
	switch (fireRandNum) {
	case 1:
		Montage_Play(AK_Fire1_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		break;
	case 2:
		Montage_Play(AK_Fire2_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		break;
	}*/
	

	fireRandNum = FMath::RandRange(1, 3);
	FString hitString = "Fire";
	hitString.AppendInt(fireRandNum);
	FName hitName = (*hitString);

	if (upperStateNClass == UArmed_PlayerUpper::StaticClass()) {
		Montage_Play(AK_Fire_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		Montage_JumpToSection(hitName, AK_Fire_Montage);
	}
	else {
		Montage_Play(AK_ADS_Fire_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		Montage_JumpToSection(hitName, AK_ADS_Fire_Montage);
	}
	
}

void UPlayerArm_AnimInstance::StopFireMontage()
{
	/*switch (fireRandNum) {
	case 1:
		Montage_JumpToSection("End", AK_Fire1_Montage);
		break;
	case 2:
		Montage_JumpToSection("End", AK_Fire2_Montage);
		break;
	}*/
}
