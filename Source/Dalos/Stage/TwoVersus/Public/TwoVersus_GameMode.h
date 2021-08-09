// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TwoVersus_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ATwoVersus_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATwoVersus_GameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class ATwoVersus_PlayerController*> AllPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class AActor*> AllPlayerStart;

	virtual void RestartPlayer(AController* NewPlayer) override;

	void CountBeginPlayer();
	void CountPlayerDead(FString Team);

	void CountWin();

	int GetRedTeamWinCount() { return RedTeamWinCount; } int GetBlueTeamWinCount() { return BlueTeamWinCount; }
	void SetRedTeamCount(int Set) { RedTeamCount = Set; } void SetBlueTeamCount(int Set) { BlueTeamCount = Set; }
	int GetRedTeamCount() { return RedTeamCount; } int GetBlueTeamCount() { return BlueTeamCount; }

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 RedTeamCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 BlueTeamCount = 0;
	UPROPERTY()
	int RedTeamWinCount = 0;
	UPROPERTY()
	int BlueTeamWinCount = 0;
	int BeginPlayer = 0;
	int WinEnd = 2;

	bool GameEnd = false;
	bool MatchEnd = false;

	FTimerHandle WinResultTimer;
	void WinResultEnd();
	FTimerHandle StartCountTimer;
	void StartCountEnd();
	FTimerHandle GameCountTimer;
	void GameCountEnd();

};
