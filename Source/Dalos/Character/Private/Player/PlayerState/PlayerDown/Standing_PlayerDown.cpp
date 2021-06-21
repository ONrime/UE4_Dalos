// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShake.h"
#include "Dalos/Player_Walk_CameraShake.h"

UStanding_PlayerDown::UStanding_PlayerDown()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;

}

UPlayerDownStateBase* UStanding_PlayerDown::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray<FInputActionKeyMapping> actionCrouch= playerInput->GetKeysForAction(TEXT("Crouch"));
	TArray<FInputActionKeyMapping> actionSplint = playerInput->GetKeysForAction(TEXT("Splint"));

	if (playerInput->IsPressed(actionCrouch[0].Key)) {
		temp = NewObject<UCrouch_PlayerDown>(this, UCrouch_PlayerDown::StaticClass());
	}
	else if (playerInput->IsPressed(actionSplint[0].Key) && player->GetInputDirForward() > 0.0f) {
		temp = NewObject<USplint_PlayerDown>(this, USplint_PlayerDown::StaticClass());
	}

	return temp;
}

UPlayerDownStateBase* UStanding_PlayerDown::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::CROUCH) {
		temp = NewObject<UCrouch_PlayerDown>(this, UCrouch_PlayerDown::StaticClass());
	}
	else if (press == EPlayerPress::SPLINT) {
		temp = NewObject<USplint_PlayerDown>(this, USplint_PlayerDown::StaticClass());
	}
	UE_LOG(LogTemp, Warning, TEXT("SendHandleInput"));
	return temp;
}

void UStanding_PlayerDown::StateStart(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Standing: StateStart"));
	pelvisZ = 60.0f;
	player->PlayerSpeed = 70.0f;
}

void UStanding_PlayerDown::StateUpdate(AMultiPlayerBase* player)
{
	FRotator interpToAngle = (player->GetControlRotation() - player->GetActorRotation()).GetNormalized();
	pitch = FMath::Abs(FMath::ClampAngle(interpToAngle.Pitch, -90.0f, 90.0f)) / 3.0f;
	
	// 카메라
	 //  카메라 위치
	cameraLoc = player->GetTransform().InverseTransformPosition(player->GetMesh()->GetSocketLocation("CameraLoc"));
	if (!player->GetIsCameraLock()) {
		player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
			, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, pelvisZ - pitch
				, GetWorld()->GetDeltaSeconds(), 10.0f)));
	}
	else {
		player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
			, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, cameraLoc.Z
				, GetWorld()->GetDeltaSeconds(), 10.0f)));
	}
	 //  카메라 회전
	float nowRoll = 0.0f;
	if (FMath::Abs(cameraMoveChagneRoll) > FMath::Abs(cameraTurnChagneRoll)) {
		nowRoll = cameraMoveChagneRoll;
	}else { nowRoll = cameraTurnChagneRoll; }
	FRotator nowSpringArmRot = player->SpringArm->GetRelativeRotation();
	player->SpringArm->SetRelativeRotation(FRotator(nowSpringArmRot.Pitch, nowSpringArmRot.Yaw
		, FMath::FInterpTo(nowSpringArmRot.Roll, nowRoll, GetWorld()->DeltaTimeSeconds, 5.0f)));
	
	//cameraRot = player
}

void UStanding_PlayerDown::StateEnd(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Standing: StateEnd"));
}

UClass* UStanding_PlayerDown::GetState()
{
	return UStanding_PlayerDown::StaticClass();
}

void UStanding_PlayerDown::TurnAtRate(AMultiPlayerBase* player, float Rate)
{
	Super::TurnAtRate(player, Rate);
	cameraTurnChagneRoll = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(-1.2f, 1.2f), Rate);
}

void UStanding_PlayerDown::PlayerMove(AMultiPlayerBase* player, float inputValue, float inputDirRight)
{
	if (player->IsLocallyControlled()) {
		if (inputValue > 0.0f) {
			walkShake = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(UPlayer_Walk_CameraShake::StaticClass(), 1.0f);
		}else { GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(walkShake); }
		cameraMoveChagneRoll = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(-1.5f, 1.5f), inputDirRight);
	}
}
