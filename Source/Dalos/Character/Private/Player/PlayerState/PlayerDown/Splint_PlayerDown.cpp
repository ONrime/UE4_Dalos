// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Dalos/CameraShake/Public/Player_Splint_CameraShake.h"

USplint_PlayerDown::USplint_PlayerDown()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerDownStateBase* USplint_PlayerDown::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray<FInputActionKeyMapping> actionCrouch = playerInput->GetKeysForAction(TEXT("Crouch"));
	TArray<FInputActionKeyMapping> actionSplint = playerInput->GetKeysForAction(TEXT("Splint"));

	if (playerInput->IsPressed(actionCrouch[0].Key)) {
		//temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass()); // 슬라이딩
	}
	else if (playerInput->IsPressed(actionSplint[0].Key)) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}

	return temp;
}

UPlayerDownStateBase* USplint_PlayerDown::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::CROUCH) {
		//temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass()); // 슬라이딩
	}
	else if (press == EPlayerPress::SPLINT) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}

	return temp;
}

void USplint_PlayerDown::StateStart(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Splint: StateStart"));
	pelvisZ = 60.0f;
	player->PlayerSpeed = 120.0f;
}

void USplint_PlayerDown::StateUpdate(AMultiPlayerBase* player)
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
	if (player->IsLocallyControlled()) {
		splintShake = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(UPlayer_Splint_CameraShake::StaticClass(), 1.0f);
	}
	FRotator nowSpringArmRot = player->SpringArm->GetRelativeRotation();
	player->SpringArm->SetRelativeRotation(FRotator(nowSpringArmRot.Pitch, nowSpringArmRot.Yaw
		, FMath::FInterpTo(nowSpringArmRot.Roll, cameraTurnChagneRoll, GetWorld()->DeltaTimeSeconds, 5.0f)));
}

void USplint_PlayerDown::StateEnd(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Splint: StateEnd"));
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(splintShake);
}

UClass* USplint_PlayerDown::GetState()
{
	return USplint_PlayerDown::StaticClass();
}

void USplint_PlayerDown::TurnAtRate(AMultiPlayerBase* player, float Rate)
{
	Super::TurnAtRate(player, Rate);
	cameraTurnChagneRoll = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(-2.2f, 2.2f), Rate);
}
