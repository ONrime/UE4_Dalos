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
	class ULobbyCount_UserWidget* LobbyCount_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* LoddingScreen_WB;

	TSubclassOf<class ULobbyMenu_UserWidget> LobbyMenu_Class;
	TSubclassOf<class UUserWidget> LoddingScreen_Class;
	TSubclassOf<class ULobbyCount_UserWidget> LobbyCount_Class;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo playerSettings;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> connectedPlayersInfo;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	FPlayerTeamInfo playerTeamInfo;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Chat, meta = (AllowPrivateAccess = "true"))
	FString senderText;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Chat, meta = (AllowPrivateAccess = "true"))
	FString senderName;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Client, Reliable, WithValidation)
	void InitialSetup(const FString& playerStatus, const FString& playerTeam); // 초기화 및 세이브 게임 정보 확인하고 서버에 업데이트 하기
	bool InitialSetup_Validate(const FString& playerStatus, const FString& playerTeam);
	void InitialSetup_Implementation(const FString& playerStatus, const FString& playerTeam);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void CallUpdate(FPlayerInfo player_Info, bool IsChangedStatus); // 캐릭터 선택, 변경시 업데이트 및 접속자 모두에게 로비메뉴를 업데이트를 하라고 알림
	bool CallUpdate_Validate(FPlayerInfo player_Info, bool IsChangedStatus);
	void CallUpdate_Implementation(FPlayerInfo player_Info, bool IsChangedStatus);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void CallTeamInfoUpdate(FPlayerTeamInfo team_Info); // 캐릭터 선택, 변경시 업데이트 및 접속자 모두에게 로비메뉴를 업데이트를 하라고 알림
	bool CallTeamInfoUpdate_Validate(FPlayerTeamInfo team_Info);
	void CallTeamInfoUpdate_Implementation(FPlayerTeamInfo team_Info);

	UFUNCTION(Client, Reliable, WithValidation)
	void SetupLobbyMenu(const FString& server_Name); // 로비 메뉴 설정 및 보여주기
	bool SetupLobbyMenu_Validate(const FString& server_Name);
	void SetupLobbyMenu_Implementation(const FString& server_Name);

	UFUNCTION(Client, Reliable, WithValidation)
	void AddPlayerInfo(); // 호출시 플레이 리스트 업데이트
	bool AddPlayerInfo_Validate();
	void AddPlayerInfo_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateLobbySettings(UTexture2D* mapImage, const FString& mapName, const FString& mapTime); // 클라이언트에게 방의 변경사항을 알려줌
	bool UpdateLobbySettings_Validate(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);
	void UpdateLobbySettings_Implementation(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowLodingScreen(); // 클라이언트에게 방의 변경사항을 알려줌
	bool ShowLodingScreen_Validate();
	void ShowLodingScreen_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void Kicked(); // 클라이언트에게 방의 변경사항을 알려줌
	bool Kicked_Validate();
	void Kicked_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateNumberOfPlayers(int16 
		s, int16 MaxPlayers); // 접속 플레이어 숫자 표시 업데이트
	bool UpdateNumberOfPlayers_Validate(int16 CurrentPlayers, int16 MaxPlayers);
	void UpdateNumberOfPlayers_Implementation(int16 CurrentPlayers, int16 MaxPlayers);

	UFUNCTION(Client, Reliable, WithValidation)
	void PlayCountDown();
	bool PlayCountDown_Validate();
	void PlayCountDown_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void StopCountDown();
	bool StopCountDown_Validate();
	void StopCountDown_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void GetChatMessage(const FString& textToSend);
	bool GetChatMessage_Validate(const FString& textToSend);
	void GetChatMessage_Implementation(const FString& textToSend);

	UFUNCTION(Client, Reliable, WithValidation) // 모든 플레이어 컨트롤러에게 체팅 업데이트
	void UpdateText(const FString& sender_Text, const FString& sender_Name);
	bool UpdateText_Validate(const FString& sender_Text, const FString& sender_Name);
	void UpdateText_Implementation(const FString& sender_Text, const FString& sender_Name);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ClearWidget();
	bool ClearWidget_Validate();
	void ClearWidget_Implementation();
	
	void SaveGameCheck();
	void UpdateList();
	void ClickRedButton();
	void ClickBlueButton();
	void TogglePlayer();
	void VisibleWidget();
	
	
protected:

private:
	
	


};
