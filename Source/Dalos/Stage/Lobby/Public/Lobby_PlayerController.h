// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Lobby_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALobby_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	ALobby_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class ULobby_Menu_UserWidget* LobbyMenu_Widget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInfo", meta = (AllowPrivateAccess = "true"))
	FPlayerInfo PlayerSetting;

	TSubclassOf<class ULobby_Menu_UserWidget> Lobby_Menu_Class;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_LoginPlayer(const FString& TeamName, const FString& State, int FirstID); // ������ ������ �����, ���� �ִٸ� �ʱ� ������ �ְ� ����Ʈ�� �ø���
	bool Client_LoginPlayer_Validate(const FString& TeamName, const FString& State, int FirstID);
	void Client_LoginPlayer_Implementation(const FString& TeamName, const FString& State, int FirstID);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendPlayerSetting(FPlayerInfo Setting);  // ������ ���̺� ����
	bool Server_SendPlayerSetting_Validate(FPlayerInfo Setting);
	void Server_SendPlayerSetting_Implementation(FPlayerInfo Setting);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeAllPlayerInfo(int Index, int Category, const FString& Change);
	bool Server_ChangeAllPlayerInfo_Validate(int Index, int Category, const FString& Change);
	void Server_ChangeAllPlayerInfo_Implementation(int Index, int Category, const FString& Change);

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_ChangePlayerInfo(FPlayerInfo Setting, int Category, const FString& Change);
	bool Client_ChangePlayerInfo_Validate(FPlayerInfo Setting, int Category, const FString& Change);
	void Client_ChangePlayerInfo_Implementation(FPlayerInfo Setting, int Category, const FString& Change);

	void LoginPlayer(const FString& TeamName, const FString& State); // (������, ���� ����) ������ ������ �����, ���� �ִٸ� �ʱ� ������ �ְ� ����Ʈ�� �ø���
	void PlayerListAdd(FPlayerInfo Setting);

	int GetSettingID() { return SettingID; }
	void SetInitSetting(FString Team, FString Room, int FirstID);  // �������� ����

protected:
	void SaveGameCheck(const FString& Team, const FString& State, int FirstID); // ó���� ���̺� Ȯ���ϱ�

	UPROPERTY(Replicated)
	FString TeamState = "";
	UPROPERTY(ReplicatedUsing = OnRep_UpdateClinetFirstPlayerList)
	FString RoomState = "";
	UPROPERTY(Replicated)
	int SettingID = 0;

	UFUNCTION()
	void OnRep_UpdateClinetFirstPlayerList();

private:




};
