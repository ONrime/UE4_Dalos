// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Lobby/Public/Lobby_Menu_UserWidget.h"
#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Dalos/Stage/Lobby/Public/Lobby_GameState.h"
#include "Kismet/GameplayStatics.h"

void ULobby_Menu_UserWidget::InitWidget_Implementation()
{

}

void ULobby_Menu_UserWidget::PlayerClickReadyButton()
{
	UE_LOG(LogTemp, Warning, TEXT("Menu_PlayerClickReadyButton"));
	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ReadyState == "NotReady") {
		Ctrl->Server_ChangeAllPlayerInfo(Ctrl->GetSettingID() - 1, 0, "Ready");
		ReadyState = "Ready";
	}
	else {
		Ctrl->Server_ChangeAllPlayerInfo(Ctrl->GetSettingID() - 1, 0, "NotReady");
		ReadyState = "NotReady";
	}
	
}

void ULobby_Menu_UserWidget::PlayerClickTeamButton(bool RedCheck)
{
	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (RedCheck) { // Blue로 바꾸기
		Ctrl->Server_ChangeAllPlayerInfo(Ctrl->GetSettingID() - 1, 1, "Blue");
		FString s = "Blue";
		UE_LOG(LogTemp, Warning, TEXT("PlayerClickTeamButton: %s"), *s);
	}
	else { // Red로 바꾸기
		Ctrl->Server_ChangeAllPlayerInfo(Ctrl->GetSettingID() - 1, 1, "Red");
		FString s = "Red";
		UE_LOG(LogTemp, Warning, TEXT("PlayerClickTeamButton: %s"), *s);
	}
}

void ULobby_Menu_UserWidget::FirstInPlayerList_Implementation(const TArray<FPlayerInfo>& Settings)
{
}
void ULobby_Menu_UserWidget::AddPlayerList_Implementation(FPlayerInfo Setting)
{
}
void ULobby_Menu_UserWidget::ChangeReady_Implementation(int FirstID, const FString& Change)
{
}
void ULobby_Menu_UserWidget::ChangeTeam_Implementation(FPlayerInfo Setting, bool RedCheck)
{
}
void ULobby_Menu_UserWidget::ChangeGameSetting_Implementation(FGameSetting Setting)
{
}

void ULobby_Menu_UserWidget::UpdateChat_Implementation(const FString& SenderText, const FString& SenderName)
{
}
