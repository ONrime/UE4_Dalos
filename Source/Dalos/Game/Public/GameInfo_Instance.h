// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameInfo_Instance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSessionDelegate);

UCLASS()
class DALOS_API UGameInfo_Instance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UGameInfo_Instance();

protected:
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName server_Name, bool succeeded);
	virtual void OnCreateSessionComplete_Lobby(FName serverName, bool succeeded);
	virtual void OnJoinSessionComplete(FName server_Name, EOnJoinSessionCompleteResult::Type type);
	virtual void OnFindSessionsComplete(bool succeeded);
	//virtual void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	virtual void HandleNetworkError(ENetworkFailure::Type FailureType, bool IsServer); // ��Ʈ��ũ ���� ó�� �� �޼��� ���
	virtual void HandleTravelError(ETravelFailure::Type FailureType); // �̵��� ���� ó�� �� �޼��� ���

	UFUNCTION(BlueprintCallable)
		void CreateServer(int32 playerNum, FName server_Name, bool is_Lan);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FSessionDelegate findSessionFaild;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FSessionDelegate findSessionSucceeded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MainMenu_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* HostMenu_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* ServerMenu_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowPrivateAccess = "true"))
	class UOptionMenu_UserWidget* OptionMenu_WB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* LodingScreen_WB;

	TSubclassOf<class UUserWidget> MainMenu_Class;
	TSubclassOf<class UUserWidget> HostMenu_Class;
	TSubclassOf<class UUserWidget> ServerMenu_Class;
	TSubclassOf<class UUserWidget> OptionMenu_Class;
	TSubclassOf<class UUserWidget> LodingScreen_Class;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowMainMenu();  // ���� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowHostMenu();  // ȣ��Ʈ �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowServerMenu();  // ���� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowOptionMenu();  // �ɼ� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
	void LaunchLobby(int32 playerNum, FName server_Name, bool Is_Lan);  // �κ� �����ϰ� ȣ��Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable, Category = "Event")
	void JoinServer();  // ������ ����
	UFUNCTION(BlueprintCallable, Category = "Event")
	void FindServer();  // ���� ã��
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowLodingScreen(); // �䱸�� �ε� ȭ�� ���
		//UFUNCTION(BlueprintCallable, Category = "Event")
	void Destroy_SessionCaller(class APlayerController PC); // ȣ�� �� ���� �ı�
	UFUNCTION(BlueprintCallable, Category = "Event")
	void Check_SaveGame();  // ���� ���� Ȯ�� �ϱ�

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerSetting", meta = (AllowPrivateAccess = "true"))
	int32 maxPlayer = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerSetting", meta = (AllowPrivateAccess = "true"))
	FName serverName;

private:
	FName lobbyName = "LobyMap";
	FString playerSettingsSave = "PlayerSettingsSave";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
	bool IsCreateSaveFile = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
	bool IsLan = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
	bool IsFindServer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
	int32 sessionsNum = 0;

	UFUNCTION(BlueprintPure)
	FString NetErrorToString(ENetworkFailure::Type FailureType);
	UFUNCTION(BlueprintPure)
	FString TravelErrorToString(ETravelFailure::Type FailureType);

};