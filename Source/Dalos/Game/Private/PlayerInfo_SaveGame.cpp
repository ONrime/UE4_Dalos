// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Game/Public/PlayerInfo_SaveGame.h"

UPlayerInfo_SaveGame::UPlayerInfo_SaveGame()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>BASE_PLAYERIMAGE(TEXT("Texture2D'/Engine/VREditor/Devices/Vive/UE4_Logo.UE4_Logo'"));
	if (BASE_PLAYERIMAGE.Succeeded()) S_PlayerInfo.PlayerImage = BASE_PLAYERIMAGE.Object;
	S_PlayerInfo.PlayerStatus = "NotReady";
}
void UPlayerInfo_SaveGame::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UGameInfo_Instance, Owner);
}