// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TwoVersus_PlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ATwoVersus_PlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FString TeamName = "Red";

	bool IsUse = false;

};
