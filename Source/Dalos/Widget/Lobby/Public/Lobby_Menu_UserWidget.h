// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "Lobby_Menu_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ULobby_Menu_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void FirstInPlayerList(const TArray<FPlayerInfo>& Settings); // ó�� ����Ʈ�� �� �� (ù �α���)
	void FirstInPlayerList_Implementation(const TArray<FPlayerInfo>& Settings);
	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerList(FPlayerInfo Setting); // �߰��� ������ �÷��̾ ����Ʈ�� �߰�
	void AddPlayerList_Implementation(FPlayerInfo Setting);
	UFUNCTION(BlueprintNativeEvent)
	void ChangeReady(int FirstID, const FString& Change); // Ready���� �ٲٱ�
	void ChangeReady_Implementation(int FirstID, const FString& Change);
	UFUNCTION(BlueprintNativeEvent)
	void ChangeTeam(FPlayerInfo Setting, bool RedCheck); // �� ���� �ٲٱ�
	void ChangeTeam_Implementation(FPlayerInfo Setting, bool RedCheck);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class ULobby_PlayerList_UserWidget* PlayerList_Widget = nullptr;

	UFUNCTION(BlueprintNativeEvent)
	void InitWidget(); // ��� ����Ʈ�� �ִ� ������ ����(�ʱ�ȭ)�Ѵ�
	void InitWidget_Implementation();

	UFUNCTION(BlueprintCallable)
	void PlayerClickReadyButton();
	UFUNCTION(BlueprintCallable)
	void PlayerClickTeamButton(bool RedCheck);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FString ReadyState = "NotReady";

};
