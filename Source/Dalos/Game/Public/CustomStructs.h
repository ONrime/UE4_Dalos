// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomStructs.generated.h"

/**
 * 
 */
class DALOS_API CustomStructs
{
public:
	CustomStructs();
	~CustomStructs();
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* playerImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACharacter* playerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* playerCharacterImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString playerStatus;


};
