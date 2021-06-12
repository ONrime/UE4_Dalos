// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Splint_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Standing_PlayerDown.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/Crouch_PlayerDown.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

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
}

void USplint_PlayerDown::StateUpdate(AMultiPlayerBase* player)
{

}

void USplint_PlayerDown::StateEnd(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Splint: StateEnd"));
}

UClass* USplint_PlayerDown::GetState()
{
	return USplint_PlayerDown::StaticClass();
}
