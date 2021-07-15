// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindServer_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UFindServer_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	TArray<FOnlineSessionSearchResult> Results;

	UFUNCTION(BlueprintCallable)
	void AddFindServerList(class UServerList_UserWidget* widget, int i);

	UFUNCTION(BlueprintCallable)
	void FindServerSucceeded();

};
