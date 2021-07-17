// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TwoVersus_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ATwoVersus_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATwoVersus_GameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class ATwoVersus_PlayerController*> AllPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class AActor*> AllPlayerStart;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void CountBeginPlayer();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 RedTeamCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 BlueTeamCount = 0;
	int BeginPlayer = 0;

};
