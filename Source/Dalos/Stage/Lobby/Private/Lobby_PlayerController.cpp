// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Dalos/Stage/Lobby/Public/Lobby_GameMode.h"
#include "Dalos/Stage/Lobby/Public/Lobby_GameState.h"
#include "Dalos/Widget/Lobby/Public/Lobby_Menu_UserWidget.h"
#include "Dalos/Game/Public/PlayerInfo_SaveGame.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ALobby_PlayerController::ALobby_PlayerController()
{
	static ConstructorHelpers::FClassFinder<ULobby_Menu_UserWidget>LOBBYMENE_WIDGET_CLASS(TEXT("WidgetBlueprint'/Game/UI/Lobby/LobbyMenu.LobbyMenu_C'"));
	if (LOBBYMENE_WIDGET_CLASS.Succeeded()) {
		Lobby_Menu_Class = LOBBYMENE_WIDGET_CLASS.Class;
		UE_LOG(LogTemp, Warning, TEXT("Find"));
	}
	bReplicates = true;
}

void ALobby_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) { // 서버(리슨)일때 작동 클라이언트는 복제 되고 나서 작동
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: %s, %s"), *TeamState, *RoomState);
		ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(Ctrl));
		UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdateClinetFirstPlayerList: %d"), State->AllPlayerInfo.Num());
		if (!LobbyMenu_Widget) LobbyMenu_Widget = CreateWidget<ULobby_Menu_UserWidget>(Ctrl, Lobby_Menu_Class);
		if (LobbyMenu_Widget) {
			LobbyMenu_Widget->FirstInPlayerList(State->AllPlayerInfo); // 자기 화면에 리스트 전달
			LobbyMenu_Widget->AddToViewport();
			LobbyMenu_Widget->InitWidget();
			SetShowMouseCursor(true);
			if (HasAuthority()) State->OnRep_GameSettingChange();
		}
		//FInputModeUIOnly mode;
		SetInputMode(FInputModeUIOnly());
	}
}

void ALobby_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ALobby_PlayerController::SaveGameCheck(const FString& TeamName, const FString& State, int FirstID)
{
	UPlayerInfo_SaveGame* LoadGameInstance = Cast<UPlayerInfo_SaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerInfo_SaveGame::StaticClass()));
	LoadGameInstance = Cast<UPlayerInfo_SaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSettingsSave", 0));
	if (LoadGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGameCheck"));
		PlayerSetting = LoadGameInstance->S_playerInfo;
		PlayerSetting.playerTeamStatus = TeamName;
		PlayerSetting.playerStatus = State;
		PlayerSetting.settingID = FirstID;
		Server_SendPlayerSetting(PlayerSetting); // 서버에 세이브 전달, 게임 스테이트에 전달(전체)
	}
	// 만약 없으면 세이브 하는 기능?
}

void ALobby_PlayerController::OnRep_UpdateClinetFirstPlayerList()
{
	// 클라이언트
	if (IsLocalController()) {
		
		/*ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(Ctrl));
		UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdateClinetFirstPlayerList: %d"), State->AllPlayerInfo.Num());
		if (!LobbyMenu_Widget) LobbyMenu_Widget = CreateWidget<ULobby_Menu_UserWidget>(Ctrl, Lobby_Menu_Class);
		if (LobbyMenu_Widget) {
			LobbyMenu_Widget->FirstInPlayerList(State->AllPlayerInfo); // 자기 화면에 리스트 전달
			LobbyMenu_Widget->AddToViewport();
			SetShowMouseCursor(true);
		}*/
	}
}

bool ALobby_PlayerController::Server_SendPlayerSetting_Validate(FPlayerInfo Setting)
{
	return true;
}
void ALobby_PlayerController::Server_SendPlayerSetting_Implementation(FPlayerInfo Setting)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_SendPlayerSetting"));
	PlayerSetting = Setting; // 서버에 설정 전달
	ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
	State->AddPlayerInfoCount = State->AddPlayerInfoCount + 1;
	State->AllPlayerInfo.Add(Setting);
	State->OnRep_AllPlayerInfoChange();

	//State->NetMultiCast_AddAllPlayerInfo(Setting); // 게임 스테이트에 세이브 추가
}

bool ALobby_PlayerController::Client_ChangePlayerInfo_Validate(FPlayerInfo Setting, int Category, const FString& Change)
{
	return true;
}
void ALobby_PlayerController::Client_ChangePlayerInfo_Implementation(FPlayerInfo Setting, int Category, const FString& Change)
{
	if (LobbyMenu_Widget) {
		ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
		if (Category == 0) { // 준비 상태
			LobbyMenu_Widget->ChangeReady(Setting.settingID, Change);
			UE_LOG(LogTemp, Warning, TEXT("Client_ChangePlayerInfo: %d"), Setting.settingID);
		}
		else { // 팀 상태
			if (Change == "Red") {
				LobbyMenu_Widget->ChangeTeam(Setting, false);
			}
			else {
				LobbyMenu_Widget->ChangeTeam(Setting, true);
			}
			UE_LOG(LogTemp, Warning, TEXT("Client_ChangePlayerInfo: %s"), *Setting.playerTeamStatus);
			UE_LOG(LogTemp, Warning, TEXT("Client_ChangePlayerInfo: %d"), Setting.settingID);
		}
	}
}

bool ALobby_PlayerController::Server_ChangeGameSetting_Validate(FGameSetting Setting)
{
	return true;
}
void ALobby_PlayerController::Server_ChangeGameSetting_Implementation(FGameSetting Setting)
{
	ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
	State->GameSetting = Setting;
}

bool ALobby_PlayerController::GetChatMessage_Validate(const FString& TextToSend)
{
	return true;
}
void ALobby_PlayerController::GetChatMessage_Implementation(const FString& TextToSend)
{
	UE_LOG(LogTemp, Warning, TEXT("GetChatMessage"));
	auto State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
	for (int i = 0; i < State->AllPlayerController.Num(); i++) {
		auto Ctrl = Cast<ALobby_PlayerController>(State->AllPlayerController[i]);
		Ctrl->UpdateText(TextToSend, PlayerSetting.playerName);
	}
}
bool ALobby_PlayerController::UpdateText_Validate(const FString& SenderText, const FString& SenderName)
{
	return true;
}
void ALobby_PlayerController::UpdateText_Implementation(const FString& SenderText, const FString& SenderName)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateText"));
	if (LobbyMenu_Widget) {
		LobbyMenu_Widget->UpdateChat(SenderText, SenderName);
	}
}

bool ALobby_PlayerController::StartCountDown_Validate()
{
	return true;
}
void ALobby_PlayerController::StartCountDown_Implementation()
{
	if (LobbyMenu_Widget) {
		UE_LOG(LogTemp, Warning, TEXT("StartCountDown: Ctrl"));
		LobbyMenu_Widget->StartCountDown();
		UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
		ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
		Ins->PlayerTeamName = PlayerSetting.playerTeamStatus;
		Ins->MatchCount = State->GameSetting.MatchCount;
		Ins->MatchTime = State->GameSetting.MatchTime;
	}
}
bool ALobby_PlayerController::StopCountDown_Validate()
{
	return true;
}
void ALobby_PlayerController::StopCountDown_Implementation()
{
	if (LobbyMenu_Widget) {
		LobbyMenu_Widget->StopCountDown();
	}
}

void ALobby_PlayerController::LoginPlayer(const FString& Team, const FString& State) // 삭제
{
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("LoginPlayer"));
		TeamState = Team;
		RoomState = State;
		//SaveGameCheck(TeamState, RoomState); // 플레이어가 가지고 있는 세이브 불러오기
	}
}

void ALobby_PlayerController::PlayerListAdd(FPlayerInfo Setting)
{
	if (IsLocalController()) {
		LobbyMenu_Widget->AddPlayerList(Setting);
	}
}

void ALobby_PlayerController::ChangeGameSetting()
{
	if (IsLocalController()) {
		UE_LOG(LogTemp, Warning, TEXT("ChangeGameSetting: Ctrl"));
		ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
		LobbyMenu_Widget->ChangeGameSetting(State->GameSetting);
	}
}

void ALobby_PlayerController::SetInitSetting(FString Team, FString Room, int FirstID) // 서버에서 동작
{
	TeamState = Team;
	RoomState = Room;
	SettingID = FirstID;
}

bool ALobby_PlayerController::Server_ChangeAllPlayerInfo_Validate(int Index, int Category, const FString& Change)
{
	return true;
}
void ALobby_PlayerController::Server_ChangeAllPlayerInfo_Implementation(int Index, int Category, const FString& Change)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_ChangeAllPlayerInfo: %s"), *Change);
	ALobby_GameState* State = Cast<ALobby_GameState>(UGameplayStatics::GetGameState(this));
	if (Category == 0) { // 0 이면 Ready를 1 이면 팀을 바꾼다
		State->AllPlayerInfo[Index].playerReadyStatus = Change;
		PlayerSetting.playerReadyStatus = Change;
		int ReadyCount = 0;
		for (int i = 0; i < State->AllPlayerInfo.Num(); i++) {
			if (State->AllPlayerInfo[i].playerReadyStatus == "Ready") {
				ReadyCount++;
			}
		}
		if (ReadyCount >= State->AllPlayerInfo.Num()) {
			// 카운트 다운
			ALobby_GameMode* GameMode = Cast<ALobby_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			GameMode->StartCountDown();
		}
	}
	else {
		State->AllPlayerInfo[Index].playerTeamStatus = Change;
		PlayerSetting.playerTeamStatus = Change;
	}
	for (int i = 0; i < State->AllPlayerController.Num(); i++) {
		ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(State->AllPlayerController[i]);
		Ctrl->Client_ChangePlayerInfo(State->AllPlayerInfo[Index], Category, Change); // ui를 바꾸기 위해
	}
}

bool ALobby_PlayerController::Client_LoginPlayer_Validate(const FString& TeamName, const FString& State, int FirstID)
{
	return true;
}
void ALobby_PlayerController::Client_LoginPlayer_Implementation(const FString& TeamName, const FString& State, int FirstID)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_LoginPlayer: %d"), FirstID);
	TeamState = TeamName;
	RoomState = State;
	SettingID = FirstID;
	SaveGameCheck(TeamState, RoomState, FirstID); // 플레이어가 가지고 있는 세이브 불러오기
}
void ALobby_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobby_PlayerController, TeamState);
	DOREPLIFETIME(ALobby_PlayerController, RoomState);
	DOREPLIFETIME(ALobby_PlayerController, SettingID);
	DOREPLIFETIME(ALobby_PlayerController, PlayerSetting);
}
