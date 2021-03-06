// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Lobby/Public/Lobby_GameSetting_UserWidget.h"
#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ULobby_GameSetting_UserWidget::SetGameSetting(FGameSetting Set)
{
	GameSetting = Set;
	BackupGameSetting = Set;
}

void ULobby_GameSetting_UserWidget::ClickAcceptButton()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickAcceptButton"));

	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Ctrl->Server_ChangeGameSetting(GameSetting);

}
