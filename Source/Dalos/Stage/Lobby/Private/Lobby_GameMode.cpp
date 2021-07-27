// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Lobby/Public/Lobby_GameMode.h"
#include "Dalos/Stage/Lobby/Public/Lobby_GameState.h"
#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Dalos/DalosCharacter.h"
#include "GameFramework/PlayerStart.h"

ALobby_GameMode::ALobby_GameMode()
{
	//DefaultPawnClass = ADalosCharacter::StaticClass();
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

		FGameSetting Setting; // ���� �ʱ�ȭ
		Setting.MapName = "TestMap";
		Setting.GameModeName = "2 VS 2";
		Setting.MatchCount = 3;
		Setting.MatchTime = 20;
		State->GameSetting = Setting;
	}// �ʱ�ȭ �� ���̺� ���� ���� Ȯ���ϰ� ������ ������Ʈ �ϱ�

	GiveID++;
	Ctrl->SetInitSetting(TeamName, HostName, GiveID);
	Ctrl->Client_LoginPlayer(TeamName, HostName, GiveID); // Ŭ���̾�Ʈ�� ����

	 // �� ��Ʈ��(Ŭ���̾�Ʈ)�� �÷��̾� �� ������Ʈ �� �÷��̾� ���� ����
	//if (!Ctrl->IsLocalController()) Ctrl->SetupLobbyMenu(gameState->serverName); // �� Ŭ���̾�Ʈ�� �κ� �޴� Ȱ��ȭ
	//Ctrl->UpdateLobbySettings(mapImage, mapName, mapTime); // �� Ŭ���̾�Ʈ�� �⺻ �� �̹���, �� �̸�, �� �ð� ���� �ؼ� ����

	

}

void ALobby_GameMode::StopCountDown()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CountDownTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimer);
		ALobby_GameState* State = Cast<ALobby_GameState>(GameState);
		if (State) {
			for (int i = 0; i < State->AllPlayerController.Num(); i++) {
				ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(State->AllPlayerController[i]);
				if (Ctrl) Ctrl->StopCountDown();
			}
		}
	}
}

void ALobby_GameMode::StartCountDown()
{
	CountNum = 5;
	GetWorld()->GetTimerManager().SetTimer(CountDownTimer, this, &ALobby_GameMode::MinCount, 1.0f, true);
	UE_LOG(LogTemp, Warning, TEXT("StartCountDown"));
	ALobby_GameState* State = Cast<ALobby_GameState>(GameState);
	if (State) {
		for (int i = 0; i < State->AllPlayerController.Num(); i++) {
			ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(State->AllPlayerController[i]);
			if (Ctrl) Ctrl->StartCountDown();
		}
	}
}

void ALobby_GameMode::MinCount()
{
	CountNum -= 1;
	if (CountNum == 0) {
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimer);
		auto gameState = Cast<ALobby_GameState>(GameState);
		UE_LOG(LogTemp, Warning, TEXT("LaunchTheGame: %d"), gameState->AllPlayerController.Num());
		for (int i = 0; i < gameState->AllPlayerController.Num(); i++) {
			ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(gameState->AllPlayerController[i]);
			//Ctrl->ClearWidget();
			UE_LOG(LogTemp, Warning, TEXT("ClearWidget"));
		}
		//GetWorld()->ServerTravel("/Game/Map/TestMap?Game=ATwoVersus_GameMode");
		GetWorld()->ServerTravel("/Game/Map/TestMap?Game=ATwoVersus_GameMode");
	}
}
