// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Loby/Public/Loby_GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Dalos/Stage/Loby/Public/Loby_PlayerController.h"
#include "Dalos/DalosCharacter.h"

ALoby_GameModeBase::ALoby_GameModeBase()
{
	SetReplicates(true);

	DefaultPawnClass = ADalosCharacter::StaticClass();
	PlayerControllerClass = ALoby_PlayerController::StaticClass();

}

bool ALoby_GameModeBase::SwapCharacter_Validate(APlayerController* controller, ACharacter* character, bool changedStatus)
{
	return true;
}

void ALoby_GameModeBase::SwapCharacter_Implementation(APlayerController* controller, ACharacter* character, bool changedStatus)
{
}

bool ALoby_GameModeBase::EveryoneUpdate_Validate()
{
	return true;
}

void ALoby_GameModeBase::EveryoneUpdate_Implementation()
{
}
