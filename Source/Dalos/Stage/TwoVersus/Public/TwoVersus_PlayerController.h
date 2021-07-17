// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TwoVersus_PlayerController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FTwoVersusControllerCheckDelegate)

UCLASS()
class DALOS_API ATwoVersus_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATwoVersus_PlayerController();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY()
	class ULobbyCount_UserWidget* Count_WB = nullptr;
	FTwoVersusControllerCheckDelegate CountDownStartCheck;

protected:
	FString TeamName = "";
	TSubclassOf<class ULobbyCount_UserWidget> Count_Class;

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
