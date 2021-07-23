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

	// 팀 분배 시키기
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
	//if (Ctrl->IsLocalController()) Ctrl->SetupLobbyMenu_Implementation(gameState->serverName); // 각 클라이언트의 로비 메뉴 활성화
	FString HostName = "Guest";
	if (!IsHost) {
		IsHost = true;
		HostName = "Host";
	}else HostName = "Guest";// 초기화 및 세이브 게임 정보 확인하고 서버에 업데이트 하기

	GiveID++;
	Ctrl->SetInitSetting(TeamName, HostName, GiveID);
	Ctrl->Client_LoginPlayer(TeamName, HostName, GiveID); // 클라이언트에 전달

	 // 각 컨트롤(클라이언트)에 플레이어 수 업데이트 및 플레이어 인포 전달
	//if (!Ctrl->IsLocalController()) Ctrl->SetupLobbyMenu(gameState->serverName); // 각 클라이언트의 로비 메뉴 활성화
	//Ctrl->UpdateLobbySettings(mapImage, mapName, mapTime); // 각 클라이언트의 기본 맵 이미지, 맵 이름, 맵 시간 전달 해서 설정

	

}