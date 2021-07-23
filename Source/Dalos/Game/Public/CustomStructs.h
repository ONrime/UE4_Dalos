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
	int settingID = 0;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString playerReadyStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString playerTeamStatus;
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerTeamInfo
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TeamName;
};

USTRUCT(Atomic, BlueprintType)
struct FVelocityBlend
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float right;
};
