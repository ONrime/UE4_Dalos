// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerArm_AnimInstance.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "PlayerBody_AnimInstance.h"
#include "PlayerArm_AnimInstance.h"

UPlayerArm_AnimInstance::UPlayerArm_AnimInstance()
{

}

void UPlayerArm_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn)) {
		auto player = Cast<AMultiPlayerBase>(pawn);
		playerSpeed = player->GetVelocity().Size();

		upperStateNClass = player->upperStateNClass;
		//UE_LOG(LogTemp, Warning, TEXT("playerSpeed: %f"), playerSpeed);
	}
}
