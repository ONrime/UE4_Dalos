// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameState.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"
#include "Dalos/Widget/Public/MultiPlayer_HUD.h"

ATwoVersus_GameMode::ATwoVersus_GameMode()
{
	DefaultPawnClass = AMultiPlayerBase::StaticClass();
	PlayerControllerClass = ATwoVersus_PlayerController::StaticClass();
	GameStateClass = ATwoVersus_GameState::StaticClass();
	PlayerStateClass = ATwoVersus_PlayerState::StaticClass();
	HUDClass = AMultiPlayer_HUD::StaticClass();
	//bUseSeamlessTravel = false;
}
