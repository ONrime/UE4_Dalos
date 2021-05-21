// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Loby_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALoby_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALoby_GameModeBase();

	// 로그인 이후 이벤트를 발동 - 플레이어 컨트롤러에서 로비를 구성하고 플레이어 스폰
	virtual void PostLogin(APlayerController* NewPlayer) override; 

	UFUNCTION(Server, Reliable, WithValidation)
	void SwapCharacter(class APlayerController* controller, class ACharacter* character, bool changedStatus);
	bool SwapCharacter_Validate(class APlayerController* controller, class ACharacter* character, bool changedStatus);
	void SwapCharacter_Implementation(class APlayerController* controller, class ACharacter* character, bool changedStatus);

	UFUNCTION(Server, Reliable, WithValidation)
	void EveryoneUpdate();
	bool EveryoneUpdate_Validate();
	void EveryoneUpdate_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(class APlayerController* controller); // 로비에 입장하면 기본 케릭터로 리스폰 - 로비 업데이트하라고 통보
	bool RespawnPlayer_Validate(class APlayerController* controller);
	void RespawnPlayer_Implementation(class APlayerController* controller);

	UFUNCTION(BlueprintCallable)
	void LaunchTheGame	();
	void LaunchCheck();

protected:

public:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class APlayerController*> allPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<class AActor*> allPlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	bool IsCasWeStart;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	FString serverName;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 maxPlayers;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	FString mapName;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	FString mapTime;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	UTexture2D* mapImage;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int32 currentPlayers = 0;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> connetedPlayers;


private:
	bool IsHost = false;
	
	int32 redTeamCount = 0;
	int32 blueTeamCount = 0;

	void SaveTeamInfo(FString status);

};
