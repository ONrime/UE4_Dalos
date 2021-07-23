// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Lobby_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALobby_GameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	ALobby_GameMode();

	// �α��� ���� �̺�Ʈ�� �ߵ� - �÷��̾� ��Ʈ�ѷ����� �κ� �����ϰ� �÷��̾� ����
	virtual void PostLogin(APlayerController* NewPlayer) override;

	
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int RedTeamCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerSettings, meta = (AllowPrivateAccess = "true"))
	int BlueTeamCount = 0;
	bool IsHost = false;
	int GiveID = 0;

};
