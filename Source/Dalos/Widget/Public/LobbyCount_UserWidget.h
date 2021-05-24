// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyCount_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ULobbyCount_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CountDown")
	ESlateVisibility countVis = ESlateVisibility::Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CountDown")
	int countNum = 5;

	UFUNCTION(BlueprintCallable)
	void PlayCountDown();
	void MinCount();
	UFUNCTION(BlueprintCallable)
	void StopCountDown();

private:
	FTimerHandle countDownTimer;

};
