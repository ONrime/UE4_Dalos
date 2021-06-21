// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerBody_AnimInstance.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerBody_AnimInstance.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"

UPlayerBody_AnimInstance::UPlayerBody_AnimInstance()
{

}

void UPlayerBody_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn)) {
		auto player = Cast<AMultiPlayerBase>(pawn);
		upperYaw = TurnBodyYaw(player);
		FRotator InterpToAngle = (player->GetControllerRot() - player->GetActorRotation()).GetNormalized();
		upperPitch = FMath::ClampAngle(InterpToAngle.Pitch, -90.0f, 90.0f);
		
		playerSpeed = player->GetVelocity().Size();
		GetVeloctyBlend(player->GetVelocity(), player->GetActorRotation(), velocityBlend);

		downStateNClass = player->downStateNClass;
		upperStateNClass = player->upperStateNClass;

		IsJumped = player->IsJumped;
		IsFalling = player->GetMovementComponent()->IsFalling();

		//UE_LOG(LogTemp, Warning, TEXT("upperPitch: %f"), player->GetControllerRot().Pitch - player->GetActorRotation().Pitch);
	}
}

float UPlayerBody_AnimInstance::TurnBodyYaw(AMultiPlayerBase* player)
{
	float yawEnd = 0.0f;
	if (player->GetVelocity().X > 0.0f || player->GetVelocity().Y > 0.0f) { // 움직일 때
		// 하체가 따라가게
		standingDir = player->GetActorRotation();
		standingDirEnd = player->GetActorRotation();
		IsTurn = false;
		//yawEnd = 0.0f;
	}
	else {
		// 하체 따로
		if (!IsTurn) {
			IsTurn = true;
			standingDirEnd = player->GetActorRotation();
		}
		else {
			if (upperYaw >= 70.0f || upperYaw <= -70.0f) {
				//UE_LOG(LogTemp, Warning, TEXT("70.0f "));
				standingDirEnd = player->GetActorRotation();
			}
			standingDir = FMath::RInterpTo(standingDir, standingDirEnd, GetWorld()->GetDeltaSeconds(), 5.0f);
			FRotator interpToAngle = (player->GetActorRotation() - standingDir).GetNormalized();
			yawEnd = FMath::ClampAngle(interpToAngle.Yaw, -90.0f, 90.0f);
			//yawEnd = interpToAngle.Yaw;
		}

	}
	
	return yawEnd;
}

FVelocityBlend UPlayerBody_AnimInstance::GetVeloctyBlend(FVector playerVelocity, FRotator playerRotator, FVelocityBlend& current)
{
	playerVelocity.Normalize();
	FMatrix RotMatrix = FRotationMatrix(playerRotator);
	FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
	FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
	FVector NormalizedVel = playerVelocity.GetSafeNormal2D();

	DirForward = FVector::DotProduct(ForwardVector, NormalizedVel);
	DirRight = FVector::DotProduct(RightVector, NormalizedVel);

	FVelocityBlend setVel;
	setVel.front = FMath::Clamp(DirForward, 0.0f, 1.0f);
	setVel.back = FMath::Abs(FMath::Clamp(DirForward, -1.0f, 0.0f));
	setVel.left = FMath::Abs(FMath::Clamp(DirRight, -1.0f, 0.0f));
	setVel.right = FMath::Clamp(DirRight, 0.0f, 1.0f);

	current.front = FMath::FInterpTo(current.front, setVel.front, GetWorld()->GetDeltaSeconds(), 8.0f);
	current.back = FMath::FInterpTo(current.back, setVel.back, GetWorld()->GetDeltaSeconds(), 8.0f);
	current.left = FMath::FInterpTo(current.left, setVel.left, GetWorld()->GetDeltaSeconds(), 8.0f);
	current.right = FMath::FInterpTo(current.right, setVel.right, GetWorld()->GetDeltaSeconds(), 8.0f);

	/*UE_LOG(LogTemp, Warning, TEXT("current.front: %f"), current.front);
	UE_LOG(LogTemp, Warning, TEXT("current.back: %f"), current.back);
	UE_LOG(LogTemp, Warning, TEXT("current.left: %f"), current.left);
	UE_LOG(LogTemp, Warning, TEXT("current.right: %f"), current.right);*/

	return FVelocityBlend();
}

void UPlayerBody_AnimInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPlayerBody_AnimInstance, upperPitch, COND_SkipOwner);
}
