// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/Lobby/Public/Lobby_GameState.h"
#include "Dalos/Stage/Lobby/Public/Lobby_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"

ALobby_GameState::ALobby_GameState()
{

}

bool ALobby_GameState::NetMultiCast_ChangePlayerInfo_Validate(int Index, int Category, const FString& Change)
{
	return true;
}
void ALobby_GameState::NetMultiCast_ChangePlayerInfo_Implementation(int Index, int Category, const FString& Change)
{
	// ���ø�����Ƽ��(����)�� �Ǿ������� ������ ���� ��Ű�� ���� rpc�� ���. (�� ������)
	//AllPlayerInfo.Add(Setting); // ����, Ŭ���̾�Ʈ�� �÷��̾� ���� ����
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("AddAllPlayerInfo: Server"));
	}else UE_LOG(LogTemp, Warning, TEXT("AddAllPlayerInfo: Client"));

	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Category == 0) { // 0 �̸� Ready�� 1 �̸� ���� �ٲ۴�
		AllPlayerInfo[Index].playerReadyStatus = Change;
	}
	else {
		AllPlayerInfo[Index].playerTeamStatus = Change;
	}
}

void ALobby_GameState::OnRep_AllPlayerInfoChange()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_AllPlayerInfoChange:%d "), AddPlayerInfoNum);
	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
	}
	if (AddPlayerInfoNum == 0) {
		AddPlayerInfoNum = AllPlayerInfo.Num();
		UE_LOG(LogTemp, Warning, TEXT("AddPlayerInfoNum: %d"), AddPlayerInfoNum);
		return;
	}
	if (AddPlayerInfoNum < AllPlayerInfo.Num()) { // �߰�(�÷��̾ ���� ��)
		AddPlayerInfoNum = AllPlayerInfo.Num();
		Ctrl->PlayerListAdd(AllPlayerInfo[AddPlayerInfoNum - 1]); // 0812���� ?Ctrl->GetSettingID() - 1
		UE_LOG(LogTemp, Warning, TEXT("PlayerAdd: %s"), *(AllPlayerInfo[AddPlayerInfoNum - 1].playerName));
		UE_LOG(LogTemp, Warning, TEXT("PlayerAdd1: %s"), *(AllPlayerInfo[0].playerName));
		UE_LOG(LogTemp, Warning, TEXT("PlayerAdd2: %s"), *(AllPlayerInfo[1].playerName));
		UE_LOG(LogTemp, Warning, TEXT("PlayerAdd3: %d"), AddPlayerInfoNum - 1);
		UE_LOG(LogTemp, Warning, TEXT("PlayerAdd4: %d"), Ctrl->GetSettingID() - 1);
	}
	else if (AddPlayerInfoNum > AllPlayerInfo.Num()) { // ����(�÷��̾ ���� ��)
		UE_LOG(LogTemp, Warning, TEXT("PlayerDel"));
		AddPlayerInfoNum = AllPlayerInfo.Num();
	}
	

}

void ALobby_GameState::OnRep_GameSettingChange()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_GameSettingChange"));
	ALobby_PlayerController* Ctrl = Cast<ALobby_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Ctrl->ChangeGameSetting();
}

void ALobby_GameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobby_GameState, AllPlayerController);
	DOREPLIFETIME(ALobby_GameState, AllPlayerInfo);
	DOREPLIFETIME(ALobby_GameState, AllPlayerStart);
	DOREPLIFETIME(ALobby_GameState, ServerName);
	DOREPLIFETIME(ALobby_GameState, MaxPlayers);
	DOREPLIFETIME(ALobby_GameState, CurrentPlayers);
	//DOREPLIFETIME(ALobby_GameState, AddPlayerInfoNum);
	DOREPLIFETIME(ALobby_GameState, GameSetting);

}