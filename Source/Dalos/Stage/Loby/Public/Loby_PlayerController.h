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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo playerSettings;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> connectedPlayersInfo;

protected:

private:
	UFUNCTION(Client, Reliable, WithValidation)
	void InitialSetup(); // 초기화 및 세이브 게임 정보 확인하고 서버에 업데이트 하기
	bool InitialSetup_Validate();
	void InitialSetup_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void CallUpdate(FPlayerInfo player_Info, bool IsChangedStatus); // 캐릭터 선택, 변경시 업데이트 및 접속자 모두에게 로비메뉴를 업데이트를 하라고 알림
	bool CallUpdate_Validate(FPlayerInfo player_Info, bool IsChangedStatus);
	void CallUpdate_Implementation(FPlayerInfo player_Info, bool IsChangedStatus);

	UFUNCTION(Client, Reliable, WithValidation)
	void SetupLobbyMenu(const FString& server_Name); // 로비 메뉴 설정 및 보여주기
	bool SetupLobbyMenu_Validate(const FString& server_Name);
	void SetupLobbyMenu_Implementation(const FString& server_Name);

	UFUNCTION(Client, Reliable, WithValidation)
	void AddPlayerInfo(const TArray<FPlayerInfo>& connectedPlayers_Info); // 호출시 플레이 리스트 업데이트
	bool AddPlayerInfo_Validate(const TArray<FPlayerInfo>& connectedPlayers_Info);
	void AddPlayerInfo_Implementation(const TArray<FPlayerInfo>& connectedPlayers_Info);

	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateLobbySettings(UTexture2D* mapImage, const FString& mapName, const FString& mapTime); // 클라이언트에게 방의 변경사항을 알려줌
	bool UpdateLobbySettings_Validate(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);
	void UpdateLobbySettings_Implementation(UTexture2D* mapImage, const FString& mapName, const FString& mapTime);

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowLodingScreen(); // 클라이언트에게 방의 변경사항을 알려줌
	bool ShowLodingScreen_Validate();
	void ShowLodingScreen_Implementation();

	void SaveGameCheck();
	


};
