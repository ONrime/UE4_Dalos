// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

UCrouch_PlayerDown::UCrouch_PlayerDown() 
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerDownStateBase* UCrouch_PlayerDown::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray<FInputActionKeyMapping> actionCrouch = playerInput->GetKeysForAction(TEXT("Crouch"));
	TArray<FInputActionKeyMapping> actionSplint = playerInput->GetKeysForAction(TEXT("Splint"));
	TArray<FInputActionKeyMapping> actionJump = playerInput->GetKeysForAction(TEXT("Jump"));

	if (playerInput->IsPressed(actionCrouch[0].Key) || playerInput->IsPressed(actionJump[0].Key)) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}
	else if (playerInput->IsPressed(actionSplint[0].Key)) {
		temp = NewObject<USplint_PlayerDown>(this, USplint_PlayerDown::StaticClass());
	}

	return temp;
}

UPlayerDownStateBase* UCrouch_PlayerDown::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::CROUCH || press == EPlayerPress::JUMP) {
		temp = NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
	}
	else if (press == EPlayerPress::SPLINT) {
		temp = NewObject<USplint_PlayerDown>(this, USplint_PlayerDown::StaticClass());
	}
	return temp;
}

void UCrouch_PlayerDown::StateStart(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Crouch: StateStart"));
	player->PlayerSpeed = 50.0f;
}

void UCrouch_PlayerDown::StateUpdate(AMultiPlayerBase* player)
{
	FRotator interpToAngle = (player->GetControlRotation() - player->GetActorRotation()).GetNormalized();
	float pitch = FMath::Abs(FMath::ClampAngle(interpToAngle.Pitch, -90.0f, 90.0f)) / 3.0f;
	float pelvisZ = 20.0f;

	cameraLoc = player->GetTransform().InverseTransformPosition(player->GetMesh()->GetSocketLocation("CameraLoc"));
	player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
		, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, pelvisZ - pitch, GetWorld()->GetDeltaSeconds(), 5.0f)));
}

void UCrouch_PlayerDown::StateEnd(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Crouch: StateEnd"));
}

UClass* UCrouch_PlayerDown::GetState()
{
	return UCrouch_PlayerDown::StaticClass();
}

UPlayerDownStateBase* UCrouch_PlayerDown::PlayerJump(AMultiPlayerBase* player)
{
	return NewObject<UStanding_PlayerDown>(this, UStanding_PlayerDown::StaticClass());
}
