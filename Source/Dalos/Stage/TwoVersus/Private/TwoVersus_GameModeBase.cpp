// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameModeBase.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"

ATwoVersus_GameModeBase::ATwoVersus_GameModeBase()
{
	DefaultPawnClass = AMultiPlayerBase::StaticClass();
	PlayerControllerClass = ATwoVersus_PlayerController::StaticClass();
	//GameStateClass = ALoby_GameState::StaticClass();
	//bUseSeamlessTravel = false;
}