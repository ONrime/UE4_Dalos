// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Lobby_GameState.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALobby_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ALobby_GameState();

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	TArray<class APlayerController*> AllPlayerController;
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_AllPlayerInfoChange, BlueprintReadWrite, Category = ServerSettings)
	TArray<FPlayerInfo> AllPlayerInfo;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	TArray<class AActor*> AllPlayerStart;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	FString ServerName;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	int MaxPlayers;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	int CurrentPlayers = 0;
	//UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings)
	//int AddPlayerInfoCount = 0;
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_GameSettingChange, BlueprintReadWrite, Category = ServerSettings)
	FGameSetting GameSetting;

	UFUNCTION(NetMultiCast, Reliable, WithValidation)
	void NetMultiCast_ChangePlayerInfo(int Index, int Category, const FString& Change);
	bool NetMultiCast_ChangePlayerInfo_Validate(int Index, int Category, const FString& Change);
	void NetMultiCast_ChangePlayerInfo_Implementation(int Index, int Category, const FString& Change);

	UFUNCTION()
	void OnRep_AllPlayerInfoChange();
	UFUNCTION()
	void OnRep_GameSettingChange();
	
	//UPROPERTY(Replicated)
	int AddPlayerInfoNum = 0;

protected:

private:

};
