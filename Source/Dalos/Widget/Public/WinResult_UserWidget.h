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
	void SetRedWin(int set) { RedWin = set; } void SetBlueWin(int set) { BlueWin = set; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility ResultTextVis = ESlateVisibility::Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResultText = "";

	UFUNCTION(BlueprintNativeEvent)
	void SetResultTextColor();
	virtual void SetResultTextColor_Implementation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	int RedWin = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Count")
	int BlueWin = 0;
};
