// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameState.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"
#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerStart.h"
#include "GameFramework/PlayerStart.h"

ATwoVersus_GameMode::ATwoVersus_GameMode()
{
	DefaultPawnClass = AMultiPlayerBase::StaticClass();
	PlayerControllerClass = ATwoVersus_PlayerController::StaticClass();
	GameStateClass = ATwoVersus_GameState::StaticClass();
	PlayerStateClass = ATwoVersus_PlayerState::StaticClass();
	HUDClass = AMultiPlayer_HUD::StaticClass();
	//bUseSeamlessTravel = false;

	//bStartPlayersAsSpectators = 0; // 자동 스폰 막기
}

/*void ATwoVersus_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}*/

void ATwoVersus_GameMode::RestartPlayer(AController* NewPlayer)
{

	
	// 컨트롤러에 팀 등록
	ATwoVersus_PlayerController* Controller = Cast<ATwoVersus_PlayerController>(NewPlayer);
	ATwoVersus_GameState* State = Cast<ATwoVersus_GameState>(GameState);
	if (NewPlayer) {
		AllPlayerController.Add(Cast<ATwoVersus_PlayerController>(NewPlayer));
		UE_LOG(LogTemp, Warning, TEXT("AllPlayerController: %d"), AllPlayerController.Num());
	}

	if (RedTeamCount <= BlueTeamCount) {
		Controller->SetTeamName("Red");
		RedTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Red"));
	}
	else {
		Controller->SetTeamName("Blue");
		BlueTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Blue"));
	}

	// 스폰하기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoVersus_PlayerStart::StaticClass(), AllPlayerStart);
	State->AllPlayerStart = AllPlayerStart;
	for (int i = 0; i < AllPlayerStart.Num(); i++) {
		ATwoVersus_PlayerStart* PlayerStart = Cast<ATwoVersus_PlayerStart>(AllPlayerStart[i]);
		if (Controller->GetTeamName() == PlayerStart->TeamName && !(PlayerStart->IsUse)) {
			UE_LOG(LogTemp, Warning, TEXT("TeamName_P: %s"), *(PlayerStart->TeamName) );
			UE_LOG(LogTemp, Warning, TEXT("TeamName_C: %s"), *(Controller->GetTeamName()));
			UE_LOG(LogTemp, Warning, TEXT("index: %d"), i);
			PlayerStart->IsUse = true;
			NewPlayer->Possess(SpawnDefaultPawnAtTransform(NewPlayer, PlayerStart->GetTransform()));
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("RestartPlayer"));

}


void ATwoVersus_GameMode::CountBeginPlayer()
{
	BeginPlayer++;
	UE_LOG(LogTemp, Warning, TEXT("BeginPlayer: %d"), BeginPlayer);
	if (BeginPlayer == AllPlayerController.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("AllPlayerControllerC: %d"), AllPlayerController.Num());
		for (int i = 0; i < AllPlayerController.Num(); i++) {
			if(AllPlayerController[i]->CountDownStartCheck.IsBound())AllPlayerController[i]->CountDownStartCheck.Execute();
		}
	}
}

void ATwoVersus_GameMode::CountPlayerDead(FString Team)
{
	if (Team == "Red") {
		RedTeamCount--;
		if (RedTeamCount == 0) {
			// 매치 앤드
			UE_LOG(LogTemp, Warning, TEXT("Match End"));
			RedTeamWinCount++;
			for (int i = 0; i < AllPlayerController.Num(); i++) {
				AllPlayerController[i]->NetMulticast_SendWinResult(RedTeamWinCount, BlueTeamCount);
			}
		}
	}
	else {
		BlueTeamCount--;
		if (BlueTeamCount == 0) {
			// 매치 앤드
			UE_LOG(LogTemp, Warning, TEXT("Match End"));
			BlueTeamWinCount++;
			for (int i = 0; i < AllPlayerController.Num(); i++) {
				AllPlayerController[i]->NetMulticast_SendWinResult(RedTeamWinCount, BlueTeamCount);
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(WinResultTimer, this, &ATwoVersus_GameMode::WinResultEnd, 4.0f, true);
}

void ATwoVersus_GameMode::WinResultEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("ReStart"));
	for (int i = 0; i < AllPlayerController.Num(); i++) {
		AllPlayerController[i]->NetMulticast_EndWinResult();
	}
	RestartGame();
}
