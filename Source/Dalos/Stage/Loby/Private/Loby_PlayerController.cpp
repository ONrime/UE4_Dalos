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
#include "Dalos/Game/Public/PlayerInfo_SaveGame.h"
#include "Dalos/Widget/Public/LobbyCount_UserWidget.h"

ALoby_PlayerController::ALoby_PlayerController()
{
	//SetReplicates(true);
	static ConstructorHelpers::FClassFinder<ULobbyMenu_UserWidget> LOBBYMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Loby/LobyMenu.LobyMenu_C'"));
	if (LOBBYMENU_WIDGET.Succeeded()) LobbyMenu_Class = LOBBYMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> LODINGSCREEN_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/LoadingScreen.LoadingScreen_C'"));
	if (LODINGSCREEN_WIDGET.Succeeded()) LoddingScreen_Class = LODINGSCREEN_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> LOBBYCOUNT_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Loby/PlayCount.PlayCount_C'"));
	if (LOBBYCOUNT_WIDGET.Succeeded()) LobbyCount_Class = LOBBYCOUNT_WIDGET.Class;
}

bool ALoby_PlayerController::CallUpdate_Validate(FPlayerInfo player_Info, bool IsChangedStatus)
{
	return true;
}
void ALoby_PlayerController::CallUpdate_Implementation(FPlayerInfo player_Info, bool IsChangedStatus)
{
	playerSettings = player_Info;
	auto gameState = Cast<ALoby_GameState>(UGameplayStatics::GetGameState(this));
	gameState->EveryoneUpdate();
	/*if (LobbyMenu_WB) {
		LobbyMenu_WB->ClearPlayerListCheck.Broadcast();
		LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast();
	}*/

	/*auto lobbyMode = Cast<ALoby_GameModeBase>(UGameplayStatics::GetGameMode(this)); //
	//lobbyMode->SwapCharacter(this, playerSettings.playerCharacter, IsChangedStatus); // 2vs2������ ĳ���͸� �ٲ� �ʿ䰡 ����.
	lobbyMode->EveryoneUpdate(); // ��ο��� ������Ʈ�� �ض�(Ŭ���̾�Ʈ)*/
}

bool ALoby_PlayerController::CallTeamInfoUpdate_Validate(FPlayerTeamInfo team_Info)
{
	return true;
}
void ALoby_PlayerController::CallTeamInfoUpdate_Implementation(FPlayerTeamInfo team_Info)
{
	playerTeamInfo = team_Info;
}


bool ALoby_PlayerController::InitialSetup_Validate(const FString& playerStatus, const FString& playerTeam)
{
	return  true;
}
void ALoby_PlayerController::InitialSetup_Implementation(const FString& playerStatus, const FString& playerTeam)
{
	SaveGameCheck(); // ���̺� ��������
	playerSettings.playerStatus = playerStatus;
	playerSettings.playerTeamStatus = playerTeam;
	playerSettings.playerReadyStatus = "Not Ready";
	CallUpdate(playerSettings, false); // ������ ���̺� ������Ʈ �ϱ�
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
	if (LobbyCount_WB == nullptr) {
		LobbyCount_WB = CreateWidget<ULobbyCount_UserWidget>(playerController, LobbyCount_Class);
		LobbyCount_WB -> AddToViewport();
	}
	FInputModeGameAndUI mode;
	mode.SetHideCursorDuringCapture(true);
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
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
			LobbyMenu_WB->ClearPlayerListCheck.Broadcast();  // ������Ʈ �� ������ �����
		}
		for (int i = 0; i < connectedPlayersInfo.Num(); i++) {
			LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast(connectedPlayersInfo[i]); // ���ο� ������ �ٽ� ������Ʈ
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
bool ALoby_PlayerController::PlayCountDown_Validate()
{
	return true;
}
void ALoby_PlayerController::PlayCountDown_Implementation()
{
	LobbyCount_WB->PlayCountDown();
}
bool ALoby_PlayerController::StopCountDown_Validate()
{
	return true;
}
void ALoby_PlayerController::StopCountDown_Implementation()
{
	LobbyCount_WB->StopCountDown();
}
bool ALoby_PlayerController::UpdateText_Validate(const FString& sender_Text, const FString& sender_Name)
{
	return true;
}
void ALoby_PlayerController::UpdateText_Implementation(const FString& sender_Text, const FString& sender_Name)
{
	if (LobbyMenu_WB->UpdateChat.IsBound()) {
		LobbyMenu_WB->UpdateChat.Broadcast(sender_Name, sender_Text);
	}
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

/*
void ALoby_PlayerController::UpdateText(const FString& sender_Text, const FString& sender_Name)
{
	if (LobbyMenu_WB->UpdateChat.IsBound()) {
		LobbyMenu_WB->UpdateChat.Broadcast(sender_Name, sender_Text);
	}
}
void ALoby_PlayerController::StopCountDown()
{
	if (LobbyCount_WB) LobbyCount_WB->StopCountDown();
}
void ALoby_PlayerController::PlayCountDown()
{
	LobbyCount_WB->PlayCountDown();
}
void ALoby_PlayerController::Kicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap", true, "listen");
}
void ALoby_PlayerController::ShowLodingScreen()
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
void ALoby_PlayerController::UpdateLobbySettings(UTexture2D* mapImage, const FString& mapName, const FString& mapTime)
{
	if (LobbyMenu_WB != nullptr) {
		LobbyMenu_WB->mapImage = mapImage;
		LobbyMenu_WB->mapName = mapName;
		LobbyMenu_WB->mapTime = mapTime;
		//LobbyMenu_WB->
	}
}
void ALoby_PlayerController::AddPlayerInfo()
{
	auto gameState = Cast<ALoby_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (gameState != nullptr && gameState->connetedPlayers.Num() > 0) {
		connectedPlayersInfo = gameState->connetedPlayers;
	}
	if (LobbyMenu_WB != nullptr) {
		/*if (LobbyMenu_WB->ClearPlayerListCheck.IsBound()) {
			LobbyMenu_WB->ClearPlayerListCheck.Broadcast();  // ������Ʈ �� ������ �����
		}
		for (int i = 0; i < connectedPlayersInfo.Num(); i++) {
			LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast(connectedPlayersInfo[i]); // ���ο� ������ �ٽ� ������Ʈ
		}////
	}
}
void ALoby_PlayerController::SetupLobbyMenu(const FString& server_Name)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupLobbyMenu"));
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	SetShowMouseCursor(true);
	if (LobbyMenu_WB == nullptr) {
		LobbyMenu_WB = CreateWidget<ULobbyMenu_UserWidget>(playerController, LobbyMenu_Class);
		LobbyMenu_WB->serverName = server_Name;
		LobbyMenu_WB->AddToViewport();
		if(LobbyMenu_WB) UE_LOG(LogTemp, Warning, TEXT("ULobbyMenu_UserWidgetd"));
	}
	if (LobbyCount_WB == nullptr) {
		LobbyCount_WB = CreateWidget<ULobbyCount_UserWidget>(playerController, LobbyCount_Class);
		LobbyCount_WB->AddToViewport();
	}
	FInputModeGameAndUI mode;
	mode.SetHideCursorDuringCapture(true);
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	mode.SetWidgetToFocus(LobbyMenu_WB->GetCachedWidget());
	SetInputMode(mode);
}
void ALoby_PlayerController::InitialSetup(const FString& playerStatus, const FString& playerTeam)
{
	SaveGameCheck(); // ���̺� ��������
	playerSettings.playerStatus = playerStatus;
	playerSettings.playerTeamStatus = playerTeam;
	playerSettings.playerReadyStatus = "Not Ready";
	CallUpdate(playerSettings, false); // ������ ���̺� ������Ʈ �ϱ�
}
void ALoby_PlayerController::UpdateNumberOfPlayers(int16 CurrentPlayers, int16 MaxPlayers)
{
	if (LobbyMenu_WB) {
		LobbyMenu_WB->playerDisplay = FString::FromInt(CurrentPlayers) + " of " + FString::FromInt(MaxPlayers);
	}
}*/



bool ALoby_PlayerController::GetChatMessage_Validate(const FString& textToSend)
{
	return true;
}
void ALoby_PlayerController::GetChatMessage_Implementation(const FString& textToSend)
{
	senderText = textToSend;
	senderName = playerSettings.playerName;
	auto gameState = Cast<ALoby_GameState>(UGameplayStatics::GetGameState(this));
	for (int i = 0; i < gameState->allPlayerController.Num(); i++) {
		auto controller = Cast<ALoby_PlayerController>(gameState->allPlayerController[i]);
		controller->UpdateText(senderText, senderName);
	}
}

void ALoby_PlayerController::SaveGameCheck()
{
	UPlayerInfo_SaveGame* LoadGameInstance = Cast<UPlayerInfo_SaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerInfo_SaveGame::StaticClass()));
	LoadGameInstance = Cast<UPlayerInfo_SaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSettingsSave", 0));
	if (LoadGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGameCheck"));
		playerSettings.playerName=LoadGameInstance->S_playerInfo.playerName;
		playerSettings.playerImage=LoadGameInstance->S_playerInfo.playerImage;
	}
	// ���� ������ ���̺� �ϴ� ���?
}

void ALoby_PlayerController::UpdateList()
{
	if(HasAuthority() && !LobbyMenu_WB) UE_LOG(LogTemp, Warning, TEXT("UpdateListdd"));
	if (LobbyMenu_WB) {
		if (LobbyMenu_WB->ClearPlayerListCheck.IsBound()) LobbyMenu_WB->ClearPlayerListCheck.Broadcast();
		for (int i = 0; i < connectedPlayersInfo.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("TeamName: %s"), *connectedPlayersInfo[i].playerTeamStatus);
			LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast(connectedPlayersInfo[i]);
		}
	}
	//LobbyMenu_WB->UpdatePlayerWindowCheck.Broadcast();
}

void ALoby_PlayerController::ClickRedButton()
{
	playerSettings.playerTeamStatus = "Red";
	CallUpdate(playerSettings, false);
}

void ALoby_PlayerController::ClickBlueButton()
{
	playerSettings.playerTeamStatus = "Blue";
	CallUpdate(playerSettings, false);
}

void ALoby_PlayerController::TogglePlayer()
{
	SetShowMouseCursor(false);
	LobbyMenu_WB->WidgetVis = ESlateVisibility::Hidden;
	FInputModeGameOnly mode;
	mode.SetConsumeCaptureMouseDown(false);
	SetInputMode(mode);
}

void ALoby_PlayerController::VisibleWidget()
{
	SetShowMouseCursor(true);
	FInputModeGameAndUI mode;
	mode.SetHideCursorDuringCapture(true);
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	mode.SetWidgetToFocus(LobbyMenu_WB->GetCachedWidget());
	LobbyMenu_WB->WidgetVis = ESlateVisibility::Visible;
	SetInputMode(mode);
}

bool ALoby_PlayerController::ClearWidget_Validate()
{
	return true;
}
void ALoby_PlayerController::ClearWidget_Implementation()
{
	if (IsLocalController()){
		LobbyMenu_WB->RemoveFromParent();
		LobbyCount_WB->RemoveFromParent();
	}
}

void ALoby_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	// ������ �ֱ�
}

void ALoby_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoby_PlayerController, playerSettings);
	DOREPLIFETIME(ALoby_PlayerController, connectedPlayersInfo);
	DOREPLIFETIME(ALoby_PlayerController, playerTeamInfo);
	DOREPLIFETIME(ALoby_PlayerController, senderText);
	DOREPLIFETIME(ALoby_PlayerController, senderName);

}
