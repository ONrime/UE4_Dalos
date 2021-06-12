// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

UUnArmed_PlayerUpper::UUnArmed_PlayerUpper()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;
}

UPlayerUpperStateBase* UUnArmed_PlayerUpper::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray <FInputActionKeyMapping> actionFirstGun = playerInput->GetKeysForAction(TEXT("FirstGun"));
	TArray <FInputActionKeyMapping> actionSecondGun = playerInput->GetKeysForAction(TEXT("SecondGun"));
	TArray <FInputActionKeyMapping> actionInteraction = playerInput->GetKeysForAction(TEXT("Interaction"));

	if (playerInput->IsPressed(actionFirstGun[0].Key) || playerInput->IsPressed(actionSecondGun[0].Key) 
		|| playerInput->IsPressed(actionInteraction[0].Key)) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UUnArmed_PlayerUpper::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::FIRSTGUN || press == EPlayerPress::SECONDGUN 
		|| press == EPlayerPress::INTERACTION) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}
	return temp;
}

void UUnArmed_PlayerUpper::StateStart(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("UnArmed: StateStart"));
}

void UUnArmed_PlayerUpper::StateUpdate(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("UnArmed: StateUpdate"));
}

void UUnArmed_PlayerUpper::StateEnd(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("UnArmed: StateEnd"));
}

UClass* UUnArmed_PlayerUpper::GetState()
{
	return UUnArmed_PlayerUpper::StaticClass();
}
