// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TwoVersus_GameState.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FGameStateCheckDelegate)

UCLASS()
class DALOS_API ATwoVersus_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATwoVersus_GameState();
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class APlayerController*> AllPlayerController;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class AActor*> AllPlayerStart;

	FGameStateCheckDelegate ChangeTeamHPCheck;
	int GetRedTeamHP() { return RedTeamHP; } int GetBlueTeamHP() { return BlueTeamHP; }

protected:
	UPROPERTY(Replicated)
	int RedTeamHP = 0;
	UPROPERTY(Replicated)
	int BlueTeamHP = 0;

public:
	int16 redTeamWin = 0;
	int16 blueTeamWin = 0;

private:


};
