// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Lobby/Public/Lobby_GameMode.h"
#include "Dalos/Stage/Lobby/Public/Lobby_GameState.h"
#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Dalos/DalosCharacter.h"

ALobby_GameMode::ALobby_GameMode()
{
	DefaultPawnClass = ADalosCharacter::StaticClass();
	PlayerControllerClass = ALobby_PlayerController::StaticClass();
	GameStateClass = ALobby_GameState::StaticClass();
	bUseSeamlessTravel = false;
}

void ALobby_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	ALobby_GameState* State = Cast<ALobby_GameState>(GameState);
	auto Instance = Cast<UGameInfo_Instance>(GetGameInstance());
	State->AllPlayerController.Add(NewPlayer);
	State->ServerName = Instance->serverName.ToString();
	State->MaxPlayers = Instance->maxPlayer; // 4
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), gameState->allPlayerStart);

	// �� �й� ��Ű��
	FString TeamName = "";
	if (RedTeamCount <= BlueTeamCount) {
		RedTeamCount++;
		TeamName = "Red";
		UE_LOG(LogTemp, Warning, TEXT("Red"));
	}
	else {
		BlueTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Blue"));
		TeamName = "Blue";
	}

	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(NewPlayer);
	//if (Ctrl->IsLocalController()) Ctrl->SetupLobbyMenu_Implementation(gameState->serverName); // �� Ŭ���̾�Ʈ�� �κ� �޴� Ȱ��ȭ
	FString HostName = "Guest";
	if (!IsHost) {
		IsHost = true;
		HostName = "Host";
	}else HostName = "Guest";// �ʱ�ȭ �� ���̺� ���� ���� Ȯ���ϰ� ������ ������Ʈ �ϱ�

	GiveID++;
	Ctrl->SetInitSetting(TeamName, HostName, GiveID);
	Ctrl->Client_LoginPlayer(TeamName, HostName, GiveID); // Ŭ���̾�Ʈ�� ����

	 // �� ��Ʈ��(Ŭ���̾�Ʈ)�� �÷��̾� �� ������Ʈ �� �÷��̾� ���� ����
	//if (!Ctrl->IsLocalController()) Ctrl->SetupLobbyMenu(gameState->serverName); // �� Ŭ���̾�Ʈ�� �κ� �޴� Ȱ��ȭ
	//Ctrl->UpdateLobbySettings(mapImage, mapName, mapTime); // �� Ŭ���̾�Ʈ�� �⺻ �� �̹���, �� �̸�, �� �ð� ���� �ؼ� ����

	

}