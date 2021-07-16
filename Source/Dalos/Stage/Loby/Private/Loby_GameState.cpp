// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Loby/Public/Loby_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Dalos/Stage/Loby/Public/Loby_PlayerController.h"
#include "Dalos/Stage/Loby/Public/Loby_GameModeBase.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Dalos/DalosCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ALoby_GameState::ALoby_GameState() 
{
	//SetReplicates(true);
}

bool ALoby_GameState::EveryoneUpdate_Validate()
{
	return true;
}

void ALoby_GameState::EveryoneUpdate_Implementation()
{
	currentPlayers = allPlayerController.Num();
	UE_LOG(LogTemp, Warning, TEXT("currentPlayers: %d"), currentPlayers);
	if (currentPlayers > 0) {
		connetedPlayers.Empty();
		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			connetedPlayers.Add(lobbyController->playerSettings);
			if (HasAuthority()) {
				OnRep_UpdateConnetedPlayers();
			}
			UE_LOG(LogTemp, Warning, TEXT("playerSettings: %s"), *connetedPlayers[i].playerReadyStatus);
			lobbyController->UpdateNumberOfPlayers(currentPlayers, maxPlayers); // �� ��Ʈ�ѷ��� �÷��̾� �� ������Ʈ �ϱ�
		}
	}
	auto gameMode = Cast<ALoby_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		gameMode->StopCountDown();
		gameMode->LaunchCheck();
	}
}
void ALoby_GameState::OnRep_UpdateConnetedPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("allPlayerController.Num(): %d"), allPlayerController.Num());
	if (allPlayerController.Num() > 0) {
		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			if (lobbyController) {
				lobbyController->AddPlayerInfo(); // ������Ʈ�� �� ��Ʈ�ѷ��� Ŭ���̾�Ʈ���� �÷��̾� ���� ����
				lobbyController->UpdateList();
				UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdateConnetedPlayers"));
			}
		}
	}
}
void ALoby_GameState::OnRep_UpdateConnetedTeam()
{
}
void ALoby_GameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoby_GameState, allPlayerController);
	DOREPLIFETIME(ALoby_GameState, connetedPlayers);
	DOREPLIFETIME(ALoby_GameState, connetedPlayersTeam);
	DOREPLIFETIME(ALoby_GameState, allPlayerStart);
	DOREPLIFETIME(ALoby_GameState, serverName);
	DOREPLIFETIME(ALoby_GameState, maxPlayers);
	DOREPLIFETIME(ALoby_GameState, currentPlayers);

}