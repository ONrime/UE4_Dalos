// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TwoVersus_GameState.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ATwoVersus_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATwoVersus_GameState();

protected:

public:
	int16 redTeamWin = 0;
	int16 blueTeamWin = 0;

private:


};