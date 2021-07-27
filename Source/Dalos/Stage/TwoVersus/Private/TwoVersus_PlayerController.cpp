// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Dalos/Widget/Public/LobbyCount_UserWidget.h"
#include "Dalos/Widget/Public/WinResult_UserWidget.h"
#include "Dalos/Widget/Public/MatchState_UserWidget.h"
#include "Dalos/Widget/Public/MultiPlayer_HUD.h"

ATwoVersus_PlayerController::ATwoVersus_PlayerController()
{
	static ConstructorHelpers::FClassFinder<ULobbyCount_UserWidget>COUNTDOWN_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/BeginPlayCount.BeginPlayCount_C'"));
	if (COUNTDOWN_WIDGET.Succeeded()) Count_Class = COUNTDOWN_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UWinResult_UserWidget>WINCOUNT_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/WinCount.WinCount_C'"));
	if (COUNTDOWN_WIDGET.Succeeded()) WinCount_Class = WINCOUNT_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UMatchState_UserWidget>MATCHSTATE_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/MatchState.MatchState_C'"));
	if (MATCHSTATE_WIDGET.Succeeded()) MatchCount_Class = MATCHSTATE_WIDGET.Class;
}

void ATwoVersus_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	//AMultiPlayer_HUD* PlayerHUD = Cast<AMultiPlayer_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	//if(PlayerHUD) PlayerHUD->BeginPlay();
	if (IsLocalController()) {
		SendGameModeCount();
		UE_LOG(LogTemp, Warning, TEXT("BeginePlay_C_C"));
		Count_WB = CreateWidget<ULobbyCount_UserWidget>(this, Count_Class);
		Count_WB->CountDownEndCheck.BindLambda([this]()->void {
			AMultiPlayerBase* Player = Cast<AMultiPlayerBase>(GetPawn());
			Player->CountDownEndCheck.Execute();
			Count_WB->RemoveFromParent();
			});
		Count_WB->AddToViewport();
		WinCount_WB = CreateWidget<UWinResult_UserWidget>(this, WinCount_Class);
		MatchState_WB = CreateWidget<UMatchState_UserWidget>(this, MatchCount_Class);
		if (MatchState_WB) {
			MatchState_WB->AddToViewport();
			MatchState_WB->RedTeamWinCount = RedTeamWinCount;
			MatchState_WB->BlueTeamWinCount = BlueTeamWinCount;
		}
	}
	
}


void ATwoVersus_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CountDownStartCheck.BindLambda([this]()->void {
		StartCountDown();
		//UE_LOG(LogTemp, Warning, TEXT("CountDownStartCheck"));
		});
	PlayerDeadCheck.BindLambda([this]()->void {
		if (HasAuthority()) {
			ATwoVersus_GameMode* GameMode = Cast<ATwoVersus_GameMode>(UGameplayStatics::GetGameMode(this));
			GameMode->CountPlayerDead(TeamName);
		}
		});
}

bool ATwoVersus_PlayerController::StartCountDown_Validate()
{
	return true;
}
void ATwoVersus_PlayerController::StartCountDown_Implementation()
{
	if (IsLocalController() && Count_WB) {
		Count_WB->PlayCountDown();
	}
}
bool ATwoVersus_PlayerController::SendGameModeCount_Validate()
{
	return true;
}
void ATwoVersus_PlayerController::SendGameModeCount_Implementation()
{
	ATwoVersus_GameMode* GameMode = Cast<ATwoVersus_GameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode) GameMode->CountBeginPlayer();
	//UE_LOG(LogTemp, Warning, TEXT("SendGameModeCount"));
}
bool ATwoVersus_PlayerController::NetMulticast_SendWinResult_Validate(int Red, int Blue, const FString& WinTeam)
{
	return true;
}
void ATwoVersus_PlayerController::NetMulticast_SendWinResult_Implementation(int Red, int Blue, const FString& WinTeam)
{
	if (IsLocalController() && WinCount_WB) {
		WinCount_WB->SetRedWin(Red);
		WinCount_WB->SetBlueWin(Blue);
		if (WinTeam != "") {
			WinCount_WB->ResultText = WinTeam;
			WinCount_WB->SetResultTextColor();
		}
		WinCount_WB->AddToViewport();
	}
}
bool ATwoVersus_PlayerController::NetMulticast_EndWinResult_Validate()
{
	return true;
}
void ATwoVersus_PlayerController::NetMulticast_EndWinResult_Implementation()
{
	if (IsLocalController()) {
		WinCount_WB->ResultTextVis = ESlateVisibility::Hidden;
		WinCount_WB->RemoveFromParent();
	}
}

bool ATwoVersus_PlayerController::Client_StartMatchCount_Validate(float EndTime)
{
	return true;
}
void ATwoVersus_PlayerController::Client_StartMatchCount_Implementation(float EndTime)
{
	if (MatchState_WB) {
		UE_LOG(LogTemp, Warning, TEXT("Client_StartMatchCount"));
		MatchState_WB->StartTimer(EndTime);
	}
}
bool ATwoVersus_PlayerController::Server_EndMatch_Validate()
{
	return true;
}
void ATwoVersus_PlayerController::Server_EndMatch_Implementation() 
{
	ATwoVersus_GameMode* GameMode = Cast<ATwoVersus_GameMode>(UGameplayStatics::GetGameMode(this));
	GameMode->CountWin();
}

void ATwoVersus_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATwoVersus_PlayerController, RedTeamWinCount);
	DOREPLIFETIME(ATwoVersus_PlayerController, BlueTeamWinCount);
}
