// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/ADS_PlayerUpper.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

UArmed_PlayerUpper::UArmed_PlayerUpper()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerUpperStateBase* UArmed_PlayerUpper::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray <FInputActionKeyMapping> actionUnArmed = playerInput->GetKeysForAction(TEXT("UnArmed"));
	TArray <FInputActionKeyMapping> actionADS = playerInput->GetKeysForAction(TEXT("ADS"));

	if (playerInput->IsPressed(actionUnArmed[0].Key)) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (playerInput->IsPressed(actionADS[0].Key)) {
		temp = NewObject<UADS_PlayerUpper>(this, UADS_PlayerUpper::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UArmed_PlayerUpper::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::UNARMED) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (press == EPlayerPress::ADS) {
		temp = NewObject<UADS_PlayerUpper>(this, UADS_PlayerUpper::StaticClass());
	}
	return temp;
}

void UArmed_PlayerUpper::StateStart(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Armed: StateStart"));
}

void UArmed_PlayerUpper::StateUpdate(AMultiPlayerBase* player)
{
}

void UArmed_PlayerUpper::StateEnd(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("Armed: StateEnd"));
}

UClass* UArmed_PlayerUpper::GetState()
{
	return UArmed_PlayerUpper::StaticClass();
}
