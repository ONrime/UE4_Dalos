// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Loby_GameState.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALoby_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ALoby_GameState();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void EveryoneUpdate();
	bool EveryoneUpdate_Validate();
	void EveryoneUpdate_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void LaunchStart();
	bool LaunchStart_Validate();
	void LaunchStart_Implementation();

protected:

public:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class APlayerController*> allPlayerController;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_UpdateConnetedPlayers, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> connetedPlayers;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class AActor*> allPlayerStart;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	FString serverName;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 maxPlayers;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 currentPlayers = 0;

	UFUNCTION()
	void OnRep_UpdateConnetedPlayers();

private:

};
