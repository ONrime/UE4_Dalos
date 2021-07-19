// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinResult_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UWinResult_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	int RedWin = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	int BlueWin = 0;

};