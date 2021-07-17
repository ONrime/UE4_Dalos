// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Dalos/Widget/Public/LobbyCount_UserWidget.h"

ATwoVersus_PlayerController::ATwoVersus_PlayerController()
{
	static ConstructorHelpers::FClassFinder<ULobbyCount_UserWidget>COUNTDOWN_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/BeginPlayCount.BeginPlayCount_C'"));
	if (COUNTDOWN_WIDGET.Succeeded()) Count_Class = COUNTDOWN_WIDGET.Class;
}

void ATwoVersus_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) {
		SendGameModeCount();
		//UE_LOG(LogTemp, Warning, TEXT("BeginePlay_C_C"));
		Count_WB = CreateWidget<ULobbyCount_UserWidget>(this, Count_Class);
		Count_WB->CountDownEndCheck.BindLambda([this]()->void {
			AMultiPlayerBase* Player = Cast<AMultiPlayerBase>(GetPawn());
			Player->CountDownEndCheck.Execute();
			});
		Count_WB->AddToViewport();
	}

}

void ATwoVersus_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CountDownStartCheck.BindLambda([this]()->void {
		StartCountDown();
		//UE_LOG(LogTemp, Warning, TEXT("CountDownStartCheck"));
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
