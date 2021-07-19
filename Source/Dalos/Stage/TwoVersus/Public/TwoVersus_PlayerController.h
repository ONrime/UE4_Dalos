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
	FTwoVersusControllerCheckDelegate CountDownStartCheck;
	FTwoVersusControllerCheckDelegate PlayerDeadCheck;

	FTwoVersusControllerCheckTwoDelegate WinCountCheck;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWinResult(int Red, int Blue);
	bool NetMulticast_SendWinResult_Validate(int Red, int Blue);
	void NetMulticast_SendWinResult_Implementation(int Red, int Blue);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_EndWinResult();
	bool NetMulticast_EndWinResult_Validate();
	void NetMulticast_EndWinResult_Implementation();


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	FString TeamName = "";
	TSubclassOf<class ULobbyCount_UserWidget> Count_Class;
	TSubclassOf<class UWinResult_UserWidget> WinCount_Class;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void StartCountDown();
	bool StartCountDown_Validate();
	void StartCountDown_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void SendGameModeCount();
	bool SendGameModeCount_Validate();
	void SendGameModeCount_Implementation();

public:
	FString GetTeamName() { return TeamName; } void SetTeamName(FString Set) { TeamName = Set; }
	

private:

};
