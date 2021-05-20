// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "LobbyMenu_UserWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, int32, SomeParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyOneDelegate, FPlayerInfo, incomingPlayerInfo);

UCLASS()
class DALOS_API ULobbyMenu_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString serverName;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FLobbyDelegate ClearPlayerListCheck;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FLobbyOneDelegate UpdatePlayerWindowCheck;

	UPROPERTY(Replicated, Category = LobbyInfo, EditAnywhere, BlueprintReadWrite)
	FString mapName;
	UPROPERTY(Replicated, Category = LobbyInfo, EditAnywhere, BlueprintReadWrite)
	FString mapTime;
	UPROPERTY(Replicated, Category = LobbyInfo, EditAnywhere, BlueprintReadWrite)
	UTexture2D* mapImage;

	UPROPERTY(Replicated, Category = LobbyInfo, EditAnywhere, BlueprintReadWrite)
	FString playerDisplay;

protected:

private:

};
