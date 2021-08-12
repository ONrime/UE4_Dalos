// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerController.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameState.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"
#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerStart.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "GameFramework/PlayerStart.h"

ATwoVersus_GameMode::ATwoVersus_GameMode()
{
	DefaultPawnClass = AMultiPlayerBase::StaticClass();
	PlayerControllerClass = ATwoVersus_PlayerController::StaticClass();
	GameStateClass = ATwoVersus_GameState::StaticClass();
	PlayerStateClass = ATwoVersus_PlayerState::StaticClass();
	HUDClass = AMultiPlayer_HUD::StaticClass();
	bUseSeamlessTravel = false;

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
	UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
	Controller->Client_GetTeamName();
	FString TeamName = Controller->GetTeamName();
	//FString TeamName = "";

	
	/*if (TeamName == "") {
		UE_LOG(LogTemp, Warning, TEXT("Team Empty"));
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
	}*/

	Controller->SetTeamName("Red");
	Controller->SetMatchInfo(Ins->MatchCount, Ins->MatchTime);
	UE_LOG(LogTemp, Warning, TEXT("RestartPlayer: MatchTime: %d"), Ins->MatchTime);
	/*if (RedTeamCount <= BlueTeamCount) {
		Controller->SetTeamName("Red");
		//RedTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Red"));
	}
	else {
		Controller->SetTeamName("Blue");
		//BlueTeamCount++;
		UE_LOG(LogTemp, Warning, TEXT("Blue"));
	}*/
	Controller->RedTeamWinCount = Ins->GetRedTeamWinCount();///
	Controller->BlueTeamWinCount = Ins->GetBlueTeamWinCount();///
	
	// 스폰하기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoVersus_PlayerStart::StaticClass(), AllPlayerStart);
	State->AllPlayerStart = AllPlayerStart;
	State->AllPlayerController.Add(Controller);
	for (int i = 0; i < AllPlayerStart.Num(); i++) {
		ATwoVersus_PlayerStart* PlayerStart = Cast<ATwoVersus_PlayerStart>(AllPlayerStart[i]);
		if (Controller->GetTeamName() == PlayerStart->TeamName && !(PlayerStart->IsUse)) {
			NewPlayer->Possess(SpawnDefaultPawnAtTransform(NewPlayer, PlayerStart->GetTransform()));
			break;
		}
	}
	MatchEnd = false;
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
		GetWorldTimerManager().SetTimer(StartCountTimer, this, &ATwoVersus_GameMode::StartCountEnd, 5.0f, true);
	}
}

void ATwoVersus_GameMode::CountPlayerDead(FString Team)
{
	UE_LOG(LogTemp, Warning, TEXT("Match End WinEnd: %d"), WinEnd);
	GetWorldTimerManager().ClearTimer(GameCountTimer);
	UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
	FString EndWinTeam = "";
	if (Team == "Red") {
		RedTeamCount--;
		UE_LOG(LogTemp, Warning, TEXT("Match End RedTeamCount: %d"), RedTeamCount);
		if (RedTeamCount == 0) {
			// 매치 앤드
			Ins->SetBlueTeamWinCount(Ins->GetBlueTeamWinCount() + 1);
			//BlueTeamWinCount++;
			UE_LOG(LogTemp, Warning, TEXT("Match End Blue: %d"), Ins->GetBlueTeamWinCount());
			UE_LOG(LogTemp, Warning, TEXT("Match End Blue: %d"), Ins->GetRedTeamWinCount());
			if (WinEnd == Ins->GetBlueTeamWinCount()) {
				EndWinTeam = "BLUE";
				GameEnd = true;
			}
			for (int i = 0; i < AllPlayerController.Num(); i++) {
				AllPlayerController[i]->NetMulticast_SendWinResult(Ins->GetRedTeamWinCount(), Ins->GetBlueTeamWinCount(), EndWinTeam);
			}
		}
	}
	else {
		BlueTeamCount--;
		UE_LOG(LogTemp, Warning, TEXT("Match End BlueTeamCount: %d"), BlueTeamCount);
		if (BlueTeamCount == 0) {
			// 매치 앤드
			Ins->SetRedTeamWinCount(Ins->GetRedTeamWinCount() + 1);
			//RedTeamWinCount++;
			UE_LOG(LogTemp, Warning, TEXT("Match End Red: %d"), Ins->GetRedTeamWinCount());
			UE_LOG(LogTemp, Warning, TEXT("Match End Red: %d"), Ins->GetBlueTeamWinCount());
			if (WinEnd == Ins->GetRedTeamWinCount()) {
				EndWinTeam = "RED";
				GameEnd = true;
			}
			for (int i = 0; i < AllPlayerController.Num(); i++) {
				AllPlayerController[i]->NetMulticast_SendWinResult(Ins->GetRedTeamWinCount(), Ins->GetBlueTeamWinCount(), EndWinTeam);
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(WinResultTimer, this, &ATwoVersus_GameMode::WinResultEnd, 4.0f, true);
}

void ATwoVersus_GameMode::CountWin() // 타이머가 끝난뒤 승패 결정
{
	UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
	if (!MatchEnd) {
		MatchEnd = true;
		FString EndWinTeam = "";
		ATwoVersus_GameState* State = Cast<ATwoVersus_GameState>(GameState);
		if (State->GetRedTeamHP() > State->GetBlueTeamHP()) {
			//RedTeamWinCount++;
			Ins->SetRedTeamWinCount(Ins->GetRedTeamWinCount() + 1);
			UE_LOG(LogTemp, Warning, TEXT("Match End Red: %d"), Ins->GetRedTeamWinCount());
			if (WinEnd == Ins->GetRedTeamWinCount()) {
				EndWinTeam = "RED";
				GameEnd = true;
			}
		}
		else if (State->GetRedTeamHP() < State->GetBlueTeamHP()) {
			//BlueTeamWinCount++;
			Ins->SetBlueTeamWinCount(Ins->GetBlueTeamWinCount() + 1);
			UE_LOG(LogTemp, Warning, TEXT("Match End Blue: %d"), Ins->GetBlueTeamWinCount());
			if (WinEnd == Ins->GetBlueTeamWinCount()) {
				EndWinTeam = "BLUE";
				GameEnd = true;
			}
		}
		for (int i = 0; i < AllPlayerController.Num(); i++) {
			AllPlayerController[i]->NetMulticast_SendWinResult(Ins->GetRedTeamWinCount(), Ins->GetBlueTeamWinCount(), EndWinTeam);
		}
		GetWorld()->GetTimerManager().SetTimer(WinResultTimer, this, &ATwoVersus_GameMode::WinResultEnd, 4.0f, true);
	}
}

void ATwoVersus_GameMode::WinResultEnd()
{
	for (int i = 0; i < AllPlayerController.Num(); i++) {
		AllPlayerController[i]->NetMulticast_EndWinResult();
	}

	if (GameEnd) {
		UE_LOG(LogTemp, Warning, TEXT("End"));
		GetWorld()->ServerTravel("/Game/Map/LobyMap?Game=ALoby_GameModeBase");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ReStart"));
		RestartGame();
	}
}

void ATwoVersus_GameMode::StartCountEnd()
{
	ATwoVersus_GameState* State = Cast<ATwoVersus_GameState>(GameState);
	State->ChangeTeamHPCheck.Execute();
	UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
	float TimeEnd = 0.0f;
	for (int i = 0; i < AllPlayerController.Num(); i++) {
		if (AllPlayerController[i]->GetPlayerStateName() == "Host") {
			TimeEnd = AllPlayerController[i]->GetMatchTime();
		}
	}
	if (TimeEnd == 0.0f) TimeEnd = 40.0f;
	GetWorldTimerManager().SetTimer(GameCountTimer, this, &ATwoVersus_GameMode::GameCountEnd, TimeEnd, true);
	for (int i = 0; i < AllPlayerController.Num(); i++) {
		ATwoVersus_PlayerController* Ctrl = Cast<ATwoVersus_PlayerController>(AllPlayerController[i]);
		UE_LOG(LogTemp, Warning, TEXT("StartCountEnddd: %d"), Ins->MatchCount);
		WinEnd = FMath::CeilToInt((float)Ins->MatchCount / 2.0f);
		Ctrl->Client_StartMatchCount(Ins->MatchTime);
	}
	GetWorldTimerManager().ClearTimer(StartCountTimer);
}

void ATwoVersus_GameMode::GameCountEnd()
{
	
}
