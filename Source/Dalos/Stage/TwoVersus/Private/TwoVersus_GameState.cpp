// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameState.h"
#include "Net/UnrealNetwork.h"

ATwoVersus_GameState::ATwoVersus_GameState()
{

}
void ATwoVersus_GameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATwoVersus_GameState, AllPlayerController);
	DOREPLIFETIME(ATwoVersus_GameState, AllPlayerStart);

}