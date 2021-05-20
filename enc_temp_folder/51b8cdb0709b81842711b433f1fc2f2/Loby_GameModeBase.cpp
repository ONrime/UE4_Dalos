// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Loby/Public/Loby_GameModeBase.h"
#include "Dalos/Stage/Loby/Public/Loby_GameState.h"
#include "Dalos/Stage/Loby/Public/Loby_PlayerController.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Net/UnrealNetwork.h"
#include "Dalos/DalosCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ALoby_GameModeBase::ALoby_GameModeBase()
{
	//SetReplicates(true);

	DefaultPawnClass = ADalosCharacter::StaticClass();
	PlayerControllerClass = ALoby_PlayerController::StaticClass();
	GameStateClass = ALoby_GameState::StaticClass();

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

	auto playerController = Cast<ALoby_PlayerController>(NewPlayer);
	if (currentPlayers == 0) {
		playerController->InitialSetup("Host");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Join Host"));
		UE_LOG(LogTemp, Warning, TEXT("Host is init"));
	}
	else {
		playerController->InitialSetup("Guest");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Join Guest"));
		UE_LOG(LogTemp, Warning, TEXT("Guest is init"));
	}// �ʱ�ȭ �� ���̺� ���� ���� Ȯ���ϰ� ������ ������Ʈ �ϱ�
	 // �� ��Ʈ��(Ŭ���̾�Ʈ)�� �÷��̾� �� ������Ʈ �� �÷��̾� ���� ����

	playerController->SetupLobbyMenu(gameState->serverName); // �� Ŭ���̾�Ʈ�� �κ� �޴� Ȱ��ȭ
	playerController->UpdateLobbySettings(mapImage, mapName, mapTime); // �� Ŭ���̾�Ʈ�� �⺻ �� �̹���, �� �̸�, �� �ð� ���� �ؼ� ����
	//RespawnPlayer(NewPlayer); // �÷��̾� ĳ���� �������ϱ�

	/*if (HasAuthority()) {
		allPlayerController.Add(NewPlayer);
		auto gameIns = Cast<UGameInfo_Instance>(GetGameInstance());
		serverName = gameIns->serverName.ToString();
		maxPlayers = gameIns->maxPlayer; // maxPlayer = 4

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), allPlayerStart);

		auto playerController = Cast<ALoby_PlayerController>(NewPlayer);
		if (currentPlayers == 0) {
			playerController->InitialSetup("Host");
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Join Host"));
			UE_LOG(LogTemp, Warning, TEXT("Host is init"));
		}else{ 
			playerController->InitialSetup("Guest");
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Join Guest"));
			UE_LOG(LogTemp, Warning, TEXT("Guest is init"));
		}
		//playerController->InitialSetup("");  // �ʱ�ȭ �� ���̺� ���� ���� Ȯ���ϰ� ������ ������Ʈ �ϱ�
		// �� ��Ʈ��(Ŭ���̾�Ʈ)�� �÷��̾� �� ������Ʈ �� �÷��̾� ���� ����

		playerController->SetupLobbyMenu(serverName); // �� Ŭ���̾�Ʈ�� �κ� �޴� Ȱ��ȭ
		playerController->UpdateLobbySettings(mapImage, mapName, mapTime); // �� Ŭ���̾�Ʈ�� �⺻ �� �̹���, �� �̸�, �� �ð� ���� �ؼ� ����
		RespawnPlayer(NewPlayer); // �÷��̾� ĳ���� �������ϱ�
	}*/
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
			lobbyController->UpdateNumberOfPlayers(currentPlayers, gameState->maxPlayers); // �� ��Ʈ�ѷ��� �÷��̾� �� ������Ʈ �ϱ�
		}

		for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
			lobbyController->AddPlayerInfo(); // ������Ʈ�� �� ��Ʈ�ѷ��� Ŭ���̾�Ʈ���� �÷��̾� ���� ����
		}
	}
	/*currentPlayers = allPlayerController.Num();
	if (currentPlayers > 0) {
		connetedPlayers.Empty();
		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			connetedPlayers.Add(lobbyController->playerSettings);
			lobbyController->UpdateNumberOfPlayers(currentPlayers, maxPlayers); // �� ��Ʈ�ѷ��� �÷��̾� �� ������Ʈ �ϱ�
		}

		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			lobbyController->AddPlayerInfo(connetedPlayers); // ������Ʈ�� �� ��Ʈ�ѷ��� Ŭ���̾�Ʈ���� �÷��̾� ���� ����
		}
	}*/
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
	EveryoneUpdate(); // ? �ٽ� �ѹ� ������Ʈ postlogin���� controller�� ���� �̹� �ߵ� �� ?
}

void ALoby_GameModeBase::LaunchTheGame()
{
	UE_LOG(LogTemp, Warning, TEXT("LaunchTheGame"));

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
