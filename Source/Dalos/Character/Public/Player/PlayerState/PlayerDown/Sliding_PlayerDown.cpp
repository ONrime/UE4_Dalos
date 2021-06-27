// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Sliding_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerBody_AnimInstance.h"
#include "Dalos/Character/Public/Player/PlayerArm_AnimInstance.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShake.h"
#include "Kismet/KismetMathLibrary.h"
#include "Dalos/Player_Walk_CameraShake.h"

USliding_PlayerDown::USliding_PlayerDown()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerDownStateBase* USliding_PlayerDown::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray<FInputActionKeyMapping> actionJump = playerInput->GetKeysForAction(TEXT("Jump"));
	TArray<FInputActionKeyMapping> actionCrouch = playerInput->GetKeysForAction(TEXT("Crouch"));

	if (playerInput->IsPressed(actionCrouch[0].Key) || playerInput->IsPressed(actionJump[0].Key)) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}

	return temp;
}

UPlayerDownStateBase* USliding_PlayerDown::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::CROUCH || press == EPlayerPress::JUMP) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}
	UE_LOG(LogTemp, Warning, TEXT("SendHandleInput"));
	return temp;
}

void USliding_PlayerDown::StateStart(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Sliding: StateStart"));
	player->PlayerSpeed = 0.0f;
	pelvisZ = 20.0f;
	player->IsMove = false;
	player->AddMovementInput(player->GetMesh()->GetRightVector(), 30.0f);
	player->GetBodyAnim()->PlaySlidingMontage();
	player->GetLegAnim()->PlaySlidingMontage();
}

void USliding_PlayerDown::StateUpdate(AMultiPlayerBase* player)
{
	FRotator interpToAngle = (player->GetControlRotation() - player->GetActorRotation()).GetNormalized();
	float pitch = FMath::Abs(FMath::ClampAngle(interpToAngle.Pitch, -90.0f, 90.0f)) / 3.0f;

	cameraLoc = player->GetTransform().InverseTransformPosition(player->GetMesh()->GetSocketLocation("CameraLoc"));
	if (!player->GetIsCameraLock()) {
		player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
			, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, pelvisZ - pitch, GetWorld()->GetDeltaSeconds(), 5.0f)));
	}
	else {
		player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
			, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, cameraLoc.Z
				, GetWorld()->GetDeltaSeconds(), 10.0f)));
	}
	slidingAngle = GetSlidingAngle(player->GetMesh()->GetComponentLocation(), player->GetMesh()->GetUpVector(), forceDir);

	float dot = FVector::DotProduct(forceDir, player->GetMesh()->GetRightVector());
	float bodyAngle = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (bodyAngle < 50.0f) slidingAngle = 90.0f;

	float addForce = FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 0.0f), FVector2D(0.0f, 30.0f), slidingAngle);
	baseForce = FMath::FInterpTo(baseForce, 0.0f + addForce, GetWorld()->DeltaTimeSeconds, 3.0f);
	//UE_LOG(LogTemp, Warning, TEXT("GetVelocity().Size(): %f"), player->GetVelocity().Size());
	//UE_LOG(LogTemp, Warning, TEXT("bodyAngle: %f"), bodyAngle);
	player->AddMovementInput(player->GetMesh()->GetRightVector(), baseForce);
}

void USliding_PlayerDown::StateEnd(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Sliding: StateEnd"));
	player->IsMove = true;
	player->GetBodyAnim()->StopMontage();
	player->GetLegAnim()->StopMontage();
}

UClass* USliding_PlayerDown::GetState()
{
	return USliding_PlayerDown::StaticClass();
}

float USliding_PlayerDown::GetSlidingAngle(FVector loc, FVector dir, FVector& slidingDir)
{
	float angle = 0.0f;
	float maxAngle = 0.0f;
	FVector startTracer = loc + dir * 50.0f;
	FVector endTracer = loc + dir * -30.0f;
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;
	bool IsHit = UKismetSystemLibrary::LineTraceSingle(this, startTracer, endTracer, ETraceTypeQuery::TraceTypeQuery4
		, false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);

	if (IsHit) {
		slidingDir = FRotationMatrix::MakeFromX(outHit.Normal).GetUnitAxis(EAxis::Z);
		float dot = FVector::DotProduct(dir, slidingDir);
		angle = FMath::RadiansToDegrees(FMath::Acos(dot));
		if (angle > 91.0f && angle > maxAngle) maxAngle = angle;
		//UE_LOG(LogTemp, Warning, TEXT("SlidingEngle: %f"), angle);
	}
	

	/*FVector StartTracer_Cover = outHit.Location;
	FVector EndTracer_Cover = outHit.Location + (FRotationMatrix::MakeFromX(outHit.Normal).GetUnitAxis(EAxis::Z) * -10.0f);

	UKismetSystemLibrary::LineTraceSingle(this, StartTracer_Cover, EndTracer_Cover, ETraceTypeQuery::TraceTypeQuery4
		, false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);*/
	//outHit.Normal
	//FRotationMatrix::MakeFromX(outHit.Normal).GetUnitAxis(EAxis::X)
	return angle;
}
