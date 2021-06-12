// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Loby/Public/Loby_GameModeBase.h"
#include "Dalos/Stage/Loby/Public/Loby_GameState.h"
#include "Dalos/Stage/Loby/Public/Loby_PlayerController.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Net/UnrealNetwork.h"
#include "Dalos/DalosCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Dalos/Game/Public/PlayerTeamInfo_SaveGame.h"

ALoby_GameModeBase::ALoby_GameModeBase()
{
	//SetReplicates(true);

	DefaultPawnClass = ADalosCharacter::StaticClass();
	PlayerControllerClass = ALoby_PlayerController::StaticClass();
	GameStateClass = ALoby_GameState::StaticClass();
	bUseSeamlessTravel = false;

}

void ALoby_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto gameState = Cast<ALoby_GameState>(GameState);
	auto gameIns = Cast<UGameInfo_Instance>(GetGameInstance());
	gameState->allPlayerController.Add(NewPlayer);
	gameState->serverName = gameIns->serverName.ToString();
	gameState->maxPlayers = gameIns->maxPlayer; // 4
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), gameState->allPlayerStart);

	FString teamInfo = "";
	if (redTeamCount <= blueTeamCount) {
		redTeamCount++;
		teamInfo = "Red";
		UE_LOG(LogTemp, Warning, TEXT("Red"));
	}
	else {
		blueTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Blue"));
		teamInfo = "Blue";
	}

	auto playerController = Cast<ALoby_PlayerController>(NewPlayer);
	if (!IsHost) {
		IsHost = true;
		playerController->InitialSetup("Host", teamInfo);
	}
	else {
		playerController->InitialSetup("Guest", teamInfo);
	}// 초기화 및 세이브 게임 정보 확인하고 서버에 업데이트 하기
	 // 각 컨트롤(클라이언트)에 플레이어 수 업데이트 및 플레이어 인포 전달

	playerController->SetupLobbyMenu(gameState->serverName); // 각 클라이언트의 로비 메뉴 활성화
	playerController->UpdateLobbySettings(mapImage, mapName, mapTime); // 각 클라이언트의 기본 맵 이미지, 맵 이름, 맵 시간 전달 해서 설정
	//RespawnPlayer(NewPlayer); // 플레이어 캐릭터 리스폰하기
}

bool ALoby_GameModeBase::SwapCharacter_Validate(APlayerController* controller, ACharacter* character, bool changedStatus)
{
	return true;
}

void ALoby_GameModeBase::SwapCharacter_Implementation(APlayerController* controller, ACharacter* character, bool changedStatus)
{
}

bool ALoby_GameModeBase::EveryoneUpdate_Validate()
{
	return true;
}

void ALoby_GameModeBase::EveryoneUpdate_Implementation()
{
	auto gameState = Cast<ALoby_GameState>(GameState);
	currentPlayers = gameState->allPlayerController.Num();
	if (currentPlayers > 0) {
		gameState->connetedPlayers.Empty();
		for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
			gameState->connetedPlayers.Add(lobbyController->playerSettings);
			lobbyController->UpdateNumberOfPlayers(currentPlayers, gameState->maxPlayers); // 각 컨트롤러에 플레이어 수 업데이트 하기
		}

		for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
			lobbyController->AddPlayerInfo(); // 업데이트된 각 컨트롤러의 클라이언트에게 플레이어 인포 전달
		}
	}
}

bool ALoby_GameModeBase::RespawnPlayer_Validate(APlayerController* controller)
{
	return true;
}

void ALoby_GameModeBase::RespawnPlayer_Implementation(APlayerController* controller)
{
	if (controller->GetPawn()) {
		controller->GetPawn()->Destroy();
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = controller;
	auto spawnPlayer = GetWorld()->SpawnActor<ADalosCharacter>(ADalosCharacter::StaticClass(), allPlayerStart[FMath::FRandRange(1, allPlayerStart.Num() - 1)]->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

	controller->Possess(spawnPlayer);
	EveryoneUpdate(); // ? 다시 한번 업데이트 postlogin에서 controller를 통해 이미 발동 됨 ?
}

void ALoby_GameModeBase::LaunchTheGame()
{
	UE_LOG(LogTemp, Warning, TEXT("CountDownGame"));
	auto gameState = Cast< ALoby_GameState>(GameState);
	for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
		auto lobbyController = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
		lobbyController->PlayCountDown();
	}
	StartCountDown();
}

void ALoby_GameModeBase::LaunchCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("LaunchCheck"));
	auto gameState = Cast<ALoby_GameState>(GameState);
	int readyCount = 0;
	if (gameState) {
		for (int i = 0; i < gameState->currentPlayers; i++) {
			if (gameState->connetedPlayers[i].playerReadyStatus == "Ready") {
				readyCount++;
			}
		}
		if(readyCount == gameState->currentPlayers) LaunchTheGame();
	}
}

void ALoby_GameModeBase::StartCountDown()
{
	countNum = 5;
	GetWorld()->GetTimerManager().SetTimer(countDownTimer, this, &ALoby_GameModeBase::MinCount, 1.0f, true);
}

void ALoby_GameModeBase::StopCountDown()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(countDownTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(countDownTimer);
	}
	auto gameState = Cast< ALoby_GameState>(GameState);
	if (gameState) {
		for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
			lobbyController->StopCountDown();
		}
	}
}

void ALoby_GameModeBase::MinCount()
{
	countNum -= 1;
	if (countNum == 0) {
		GetWorld()->GetTimerManager().ClearTimer(countDownTimer);
		UE_LOG(LogTemp, Warning, TEXT("LaunchTheGame"));
		GetWorld()->ServerTravel("/Game/Map/TestMap?Game=ATwoVersus_GameModeBase", true, false);
		//GetWorld()->Exec(GetWorld(), TEXT("servertravel /Game/Map/TestMap"));
	}
}


void ALoby_GameModeBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoby_GameModeBase, serverName);
	DOREPLIFETIME(ALoby_GameModeBase, maxPlayers);
	DOREPLIFETIME(ALoby_GameModeBase, connetedPlayers);
	DOREPLIFETIME(ALoby_GameModeBase, currentPlayers);
	DOREPLIFETIME(ALoby_GameModeBase, mapName);
	DOREPLIFETIME(ALoby_GameModeBase, mapTime);
	DOREPLIFETIME(ALoby_GameModeBase, mapImage);
	DOREPLIFETIME(ALoby_GameModeBase, allPlayerController);

}
