// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchState_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UMatchState_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	float Count = 40.0f;
	UPROPERTY(BlueprintReadOnly)
	int RedTeamWinCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int BlueTeamWinCount = 0;

	void StartTimer(float Time);
	void StopTimer();

private:

	FTimerHandle MatchTimer;
	void MatchCount();
	
};
