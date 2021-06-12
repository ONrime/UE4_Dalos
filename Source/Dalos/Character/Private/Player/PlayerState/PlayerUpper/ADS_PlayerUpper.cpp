// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/ADS_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

UADS_PlayerUpper::UADS_PlayerUpper()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerUpperStateBase* UADS_PlayerUpper::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray <FInputActionKeyMapping> actionUnArmed = playerInput->GetKeysForAction(TEXT("UnArmed"));
	TArray <FInputActionKeyMapping> actionADS = playerInput->GetKeysForAction(TEXT("ADS"));

	if (playerInput->IsPressed(actionUnArmed[0].Key)) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (playerInput->InputKey(actionADS[0].Key, EInputEvent::IE_Released, 1.0f, true)) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UADS_PlayerUpper::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::UNARMED) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (press == EPlayerPress::ADS) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}
	return temp;
}

void UADS_PlayerUpper::StateStart(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("ADS: StateStart"));
}

void UADS_PlayerUpper::StateUpdate(AMultiPlayerBase* player)
{
}

void UADS_PlayerUpper::StateEnd(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("ADS: StateEnd"));
}

UClass* UADS_PlayerUpper::GetState()
{
	return UADS_PlayerUpper::StaticClass();
}
