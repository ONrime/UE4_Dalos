// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitCheck_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UHitCheck_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void RedCheck(bool check);
	virtual void RedCheck_Implementation(bool check);

};
