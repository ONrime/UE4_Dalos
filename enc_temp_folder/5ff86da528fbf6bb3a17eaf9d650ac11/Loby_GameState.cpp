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
	//UE_LOG(LogTemp, Warning, TEXT("currentPlayers: %d"), currentPlayers);
	if (currentPlayers > 0) {
		connetedPlayers.Empty();
		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			connetedPlayers.Add(lobbyController->playerSettings);
			UE_LOG(LogTemp, Warning, TEXT("playerSettings: %s"), *connetedPlayers[i].playerReadyStatus);
			lobbyController->UpdateNumberOfPlayers(currentPlayers, maxPlayers); // 각 컨트롤러에 플레이어 수 업데이트 하기
		}
	}
}
bool ALoby_GameState::LaunchStart_Validate()
{
	return true;
}
void ALoby_GameState::LaunchStart_Implementation()
{
	auto gameMode = Cast<ALoby_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		UE_LOG(LogTemp, Warning, TEXT("LaunchStart"));
		gameMode->LaunchCheck();
	}
}
void ALoby_GameState::OnRep_UpdateConnetedPlayers()
{
	if (!HasAuthority() && allPlayerController.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Change"));
		for (int i = 0; i < allPlayerController.Num(); i++) {
			auto lobbyController = Cast<ALoby_PlayerController>(allPlayerController[i]);
			if (lobbyController) {
				lobbyController->AddPlayerInfo(); // 업데이트된 각 컨트롤러의 클라이언트에게 플레이어 인포 전달
			}
		}
	}
}
void ALoby_GameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoby_GameState, allPlayerController);
	DOREPLIFETIME(ALoby_GameState, connetedPlayers);
	DOREPLIFETIME(ALoby_GameState, allPlayerStart);
	DOREPLIFETIME(ALoby_GameState, serverName);
	DOREPLIFETIME(ALoby_GameState, maxPlayers);
	DOREPLIFETIME(ALoby_GameState, currentPlayers);

}