// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHitCheck_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UPlayerHitCheck_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void HitLocCheck(FVector loc);
	virtual void HitLocCheck_Implementation(FVector loc);

};
