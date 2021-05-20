// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Loby/Public/Loby_PlayerController.h"
#include "Dalos/Stage/Loby/Public/Loby_GameModeBase.h"
#include "Dalos/Stage/Loby/Public/Loby_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Dalos/Widget/Public/LobbyMenu_UserWidget.h"

ALoby_PlayerController::ALoby_PlayerController()
{
	//SetReplicates(true);
	static ConstructorHelpers::FClassFinder<ULobbyMenu_UserWidget> LOBBYMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Loby/LobyMenu.LobyMenu_C'"));
	if (LOBBYMENU_WIDGET.Succeeded()) LobbyMenu_Class = LOBBYMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> LODINGSCREEN_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/LoadingScreen.LoadingScreen_C'"));
	if (LODINGSCREEN_WIDGET.Succeeded()) LoddingScreen_Class = LODINGSCREEN_WIDGET.Class;
}

bool ALoby_PlayerController::InitialSetup_Validate(const FString& playerStatus)
{
	return  true;
}

void ALoby_PlayerController::InitialSetup_Implementation(const FString& playerStatus)
{
	SaveGameCheck(); // 세이브 가져오기
	playerSettings.playerStatus = playerStatus;
	CallUpdate(playerSettings, false); // 가져온 세이브 업데이트 하기
}

bool ALoby_PlayerController::CallUpdate_Validate(FPlayerInfo player_Info, bool IsChangedStatus)
{
	return true;
}

void ALoby_PlayerController::CallUpdate_Implementation(FPlayerInfo player_Info, bool IsChangedStatus)
{
	if (!HasAuthority()) {
		
	}
	playerSettings = player_Info;
	auto gameState = Cast<ALoby_GameState>(UGameplayStatics::GetGameState(this));
	gameState->EveryoneUpdate();

	/*auto lobbyMode = Cast<ALoby_GameModeBase>(UGameplayStatics::GetGameMode(this)); //
	//lobbyMode->SwapCharacter(this, playerSettings.playerCharacter, IsChangedStatus); // 2vs2에서는 캐릭터를 바꿀 필요가 없다.
	lobbyMode->EveryoneUpdate(); // 모두에게 업데이트를 해라(클라이언트)*/
}

bool ALoby_PlayerController::SetupLobbyMenu_Validate(const FString& server_Name)
{
	return true;
}

void ALoby_PlayerController::SetupLobbyMenu_Implementation(const FString& server_Name)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	SetShowMouseCursor(true);
	if (LobbyMenu_WB == nullptr) {
		LobbyMenu_WB = CreateWidget<ULobbyMenu_UserWidget>(playerController, LobbyMenu_Class);
		LobbyMenu_WB->serverName = server_Name;
		LobbyMenu_WB->AddToViewport();
	}
	FInputModeGameAndUI mode;
	mode.SetHideCursorDuringCapture(true);
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	mode.SetWidgetToFocus(LobbyMenu_WB->GetCachedWidget());
	SetInputMode(mode);
}

bool ALoby_PlayerController::AddPlayerInfo_Validate()
{
	return true;
}

void ALoby_PlayerController::AddPlayerInfo_Implementation()
{
	auto gameState = Cast<ALoby_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (gameState != nullptr && gameState->connetedPlayers.Num() > 0) {
		connectedPlayersInfo = gameState->connetedPlayers;
	}
	if (LobbyMenu_WB != nullptr) {
		/*if (LobbyMenu_WB->ClearPlayerListCheck.IsBound()) {
			LobbyMenu_WB->ClearPlayerListCheck.Broadcast();  // 업데이트 시 내용을 지우고
		}
		for (int i = 0; i < connectedPlayersInfo.Num(); i++) {
			LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast(connectedPlayersInfo[i]); // 새로운 내용을 다시 업데이트
		}*/
	}
	
}

bool ALoby_PlayerController::UpdateLobbySettings_Validate(UTexture2D* mapImage, const FString& mapName, const FString& mapTime)
{
	return true;
}

void ALoby_PlayerController::UpdateLobbySettings_Implementation(UTexture2D* mapImage, const FString& mapName, const FString& mapTime)
{
	if (LobbyMenu_WB != nullptr) {
		LobbyMenu_WB->mapImage = mapImage;
		LobbyMenu_WB->mapName = mapName;
		LobbyMenu_WB->mapTime = mapTime;
		//LobbyMenu_WB->
	}
}

bool ALoby_PlayerController::ShowLodingScreen_Validate()
{
	return true;
}

void ALoby_PlayerController::ShowLodingScreen_Implementation()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (LobbyMenu_WB != nullptr) {
		LobbyMenu_WB->RemoveFromParent();
		if (LobbyMenu_WB == nullptr) {
			LobbyMenu_WB = CreateWidget<ULobbyMenu_UserWidget>(playerController, LoddingScreen_Class);
			LobbyMenu_WB->AddToViewport();
		}
	}
}

bool ALoby_PlayerController::Kicked_Validate()
{
	return true;
}

void ALoby_PlayerController::Kicked_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap", true, "listen");
}

void ALoby_PlayerController::SaveGameCheck()
{
}

bool ALoby_PlayerController::UpdateNumberOfPlayers_Validate(int16 CurrentPlayers, int16 MaxPlayers)
{
	return true;
}

void ALoby_PlayerController::UpdateNumberOfPlayers_Implementation(int16 CurrentPlayers, int16 MaxPlayers)
{
	if (LobbyMenu_WB) {
		LobbyMenu_WB->playerDisplay = FString::FromInt(CurrentPlayers) + " of " + FString::FromInt(MaxPlayers);
	}
}

void ALoby_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	// 나가기 넣기
}

void ALoby_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoby_PlayerController, playerSettings);
	DOREPLIFETIME(ALoby_PlayerController, connectedPlayersInfo);

}