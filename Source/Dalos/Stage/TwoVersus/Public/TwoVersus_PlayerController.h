// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TwoVersus_PlayerController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FTwoVersusControllerCheckDelegate)
DECLARE_DELEGATE_TwoParams(FTwoVersusControllerCheckTwoDelegate, int, int)

UCLASS()
class DALOS_API ATwoVersus_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATwoVersus_PlayerController();

	UPROPERTY()
	class ULobbyCount_UserWidget* Count_WB = nullptr;
	UPROPERTY()
	class UWinResult_UserWidget* WinCount_WB = nullptr;
	UPROPERTY()
	class UMatchState_UserWidget* MatchState_WB = nullptr;

	FTwoVersusControllerCheckDelegate CountDownStartCheck;
	FTwoVersusControllerCheckDelegate PlayerDeadCheck;

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_GetTeamName();
	bool Client_GetTeamName_Validate();
	void Client_GetTeamName_Implementation();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GetTeamName(const FString& Team);
	bool Server_GetTeamName_Validate(const FString& Team);
	void Server_GetTeamName_Implementation(const FString& Team);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWinResult(int Red, int Blue, const FString& WinTeam);
	bool NetMulticast_SendWinResult_Validate(int Red, int Blue, const FString& WinTeam);
	void NetMulticast_SendWinResult_Implementation(int Red, int Blue, const FString& WinTeam);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_EndWinResult();
	bool NetMulticast_EndWinResult_Validate();
	void NetMulticast_EndWinResult_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_StartMatchCount(float EndTime);
	bool Client_StartMatchCount_Validate(float EndTime);
	void Client_StartMatchCount_Implementation(float EndTime);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndMatch();
	bool Server_EndMatch_Validate();
	void Server_EndMatch_Implementation();

	UPROPERTY(Replicated)
	int RedTeamWinCount = 0;
	UPROPERTY(Replicated)
	int BlueTeamWinCount = 0;


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(Replicated)
	FString TeamName = "";
	UPROPERTY(Replicated)
	FString PlayerStateName = "";
	UPROPERTY(Replicated)
	int MatchCount = 0;
	UPROPERTY(Replicated)
	int MatchTime = 0;
	TSubclassOf<class ULobbyCount_UserWidget> Count_Class;
	TSubclassOf<class UWinResult_UserWidget> WinCount_Class;
	TSubclassOf<class UMatchState_UserWidget> MatchCount_Class;

	UFUNCTION(Client, Reliable, WithValidation)
	void StartCountDown();
	bool StartCountDown_Validate();
	void StartCountDown_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void SendGameModeCount();
	bool SendGameModeCount_Validate();
	void SendGameModeCount_Implementation();

public:
	FString GetTeamName() { return TeamName; } void SetTeamName(FString Set) { TeamName = Set; }
	FString GetPlayerStateName() { return PlayerStateName; } void SetPlayerStateName(FString Set) { PlayerStateName = Set; }
	int GetMatchTime() { return MatchTime; } int GetMatchCount() { return MatchCount; }
	void SetMatchInfo(int Count, int Time);

private:

};
