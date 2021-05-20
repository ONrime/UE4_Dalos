// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Loby_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALoby_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALoby_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	class ULobbyMenu_UserWidget* LobbyMenu_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* LoddingScreen_WB;

	TSubclassOf<class ULobbyMenu_UserWidget> LobbyMenu_Class;
	TSubclassOf<class UUserWidget> LoddingScreen_Class;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo playerSettings;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> connectedPlayersInfo;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Client, Reliable, WithValidation)
	void InitialSetup(const FString& playerStatus); // �ʱ�ȭ �� ���̺� ���� ���� Ȯ���ϰ� ������ ������Ʈ �ϱ�
	bool InitialSetup_Validate(const FString& playerStatus);
	void InitialSetup_Implementation(const FString& playerStatus);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void CallUpdate(FPlayerInfo player_Info, bool IsChangedStatus); // ĳ���� ����, ����� ������Ʈ �� ������ ��ο��� �κ�޴��� ������Ʈ�� �϶�� �˸�
	bool CallUpdate_Validate(FPlayerInfo player_Info, bool IsChangedStatus);
	void CallUpdate_Implementation(FPlayerInfo player_Info, bool IsChangedStatus);

	UFUNCTION(Client, Reliable, WithValidation)
	void SetupLobbyMenu(const FString& server_Name); // �κ� �޴� ���� �� �����ֱ�
	bool SetupLobbyMenu_Validate(const FString& server_Name);
	void SetupLobbyMenu_Implementation(const FString& server_Name);

	UFUNCTION(Client, Reliable, WithValidation)
	void AddPlayerInfo(); // ȣ��� �÷��� ����Ʈ ������Ʈ
	bool AddPlayerInfo_Validate();
	void AddPlayerInfo_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateLobbySettings(UTexture2D* mapImage, const FString& mapName, const FString& mapTime); // Ŭ���̾�Ʈ���� ���� ��������� �˷���
	bool UpdateLobbySettings_Validate(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);
	void UpdateLobbySettings_Implementation(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowLodingScreen(); // Ŭ���̾�Ʈ���� ���� ��������� �˷���
	bool ShowLodingScreen_Validate();
	void ShowLodingScreen_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void Kicked(); // Ŭ���̾�Ʈ���� ���� ��������� �˷���
	bool Kicked_Validate();
	void Kicked_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateNumberOfPlayers(int16 CurrentPlayers, int16 MaxPlayers); // ���� �÷��̾� ���� ǥ�� ������Ʈ
	bool UpdateNumberOfPlayers_Validate(int16 CurrentPlayers, int16 MaxPlayers);
	void UpdateNumberOfPlayers_Implementation(int16 CurrentPlayers, int16 MaxPlayers);

	void SaveGameCheck();
	

protected:

private:
	
	


};
