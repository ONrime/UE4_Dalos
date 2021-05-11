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
		FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* PlayerImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* PlayerCharacterImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerStatus;
};
