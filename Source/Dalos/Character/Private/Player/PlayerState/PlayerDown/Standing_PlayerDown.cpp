// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

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
	else if (playerInput->IsPressed(actionSplint[0].Key)) {
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
}

void UStanding_PlayerDown::StateUpdate(AMultiPlayerBase* player)
{
	/*if (player->HasAuthority()) { // 플레이어 전체 회전 (서버)
		if (player->IsLocallyControlled()) {
		}
		else {

		}
		UE_LOG(LogTemp, Warning, TEXT("server_Update: %s yawSpeed %f"), *GetName(), yawSpeed);
	}
	else { // (클라이언트)
		if (player->IsLocallyControlled()) {

		}
		else {

		}
		UE_LOG(LogTemp, Warning, TEXT("cla_Update: %s yawSpeed %f"), *GetName(), yawSpeed);
	}*/
	FRotator interpToAngle = (player->GetControlRotation() - player->GetActorRotation()).GetNormalized();
	float pitch = FMath::Abs(FMath::ClampAngle(interpToAngle.Pitch, -90.0f, 90.0f)) / 3.0f;
	float pelvisZ = 60.0f;

	cameraLoc = player->GetTransform().InverseTransformPosition(player->GetMesh()->GetSocketLocation("CameraLoc"));
	player->SpringArm->SetRelativeLocation(FVector(cameraLoc.X, cameraLoc.Y
		, FMath::FInterpTo(player->SpringArm->GetRelativeLocation().Z, pelvisZ - pitch, GetWorld()->GetDeltaSeconds(), 5.0f)));
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
	
}
