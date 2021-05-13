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
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName Server_Name, bool Succeeded);
	virtual void OnCreateSessionComplete_Lobby(FName Server_Name, bool Succeeded);
	virtual void OnJoinSessionComplete(FName Server_Name, EOnJoinSessionCompleteResult::Type type);
	virtual void OnFindSessionsComplete(bool Succeeded);
	//virtual void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	virtual void HandleNetworkError(ENetworkFailure::Type FailureType, bool bIsServer); // ��Ʈ��ũ ���� ó�� �� �޼��� ���
	virtual void HandleTravelError(ETravelFailure::Type FailureType); // �̵��� ���� ó�� �� �޼��� ���

	UFUNCTION(BlueprintCallable)
		void CreateServer(int32 Player_Num, FName Server_Name, bool Lan);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FSessionDelegate FindSessionFaild;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FSessionDelegate FindSessionSucceeded;

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
		void Show_MainMenu();  // ���� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Show_HostMenu();  // ȣ��Ʈ �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Show_ServerMenu();  // ���� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Show_OptionMenu();  // �ɼ� �޴� ȭ�� ��ȯ and ���콺 Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Launch_Lobby(int32 Player_Num, FName Server_Name, bool Lan);  // �κ� �����ϰ� ȣ��Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Join_Server();  // ������ ����
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Find_Server();  // ���� ã��
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Show_LodingScreen(); // �䱸�� �ε� ȭ�� ���
		//UFUNCTION(BlueprintCallable, Category = "Event")
	void Destroy_SessionCaller(class APlayerController PC); // ȣ�� �� ���� �ı�
	UFUNCTION(BlueprintCallable, Category = "Event")
		void Check_SaveGame();  // ���� ���� Ȯ�� �ϱ�

private:
	FName LobbyName = "LobyMap";
	FString PlayerSettingsSave = "PlayerSettingsSave";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
		bool IsCreateSaveFile = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
		bool IsLan = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
		bool IsFindServer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check", meta = (AllowPrivateAccess = "true"))
		int32 SessionsNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerSetting", meta = (AllowPrivateAccess = "true"))
		int32 MaxPlayer = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerSetting", meta = (AllowPrivateAccess = "true"))
		FName ServerName;

	UFUNCTION(BlueprintPure)
		FString NetErrorToString(ENetworkFailure::Type FailureType);
	UFUNCTION(BlueprintPure)
		FString TravelErrorToString(ETravelFailure::Type FailureType);

};