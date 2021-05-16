// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Game/Public/GameInfo_Instance.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/SlateWrapperTypes.h"
#include "Dalos/Widget/Public/OptionMenu_UserWidget.h"


UGameInfo_Instance::UGameInfo_Instance()
{
	//bReplicates = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> MAINMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/MainMenu/MainMenu.MainMenu_C'"));
	if (MAINMENU_WIDGET.Succeeded()) MainMenu_Class = MAINMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> HOSTMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/MainMenu/HostMenu.HostMenu_C'"));
	if (HOSTMENU_WIDGET.Succeeded()) HostMenu_Class = HOSTMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> SERVERMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/MainMenu/ServerMenu.ServerMenu_C'"));
	if (SERVERMENU_WIDGET.Succeeded()) ServerMenu_Class = SERVERMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UOptionMenu_UserWidget> OPTIONMENU_WIDGET(TEXT("WidgetBlueprint'/Game/UI/MainMenu/OptionMenus.OptionMenus_C'"));
	if (OPTIONMENU_WIDGET.Succeeded()) OptionMenu_Class = OPTIONMENU_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> LOADINGSCREEN_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Levels/LoadingScreen.LoadingScreen_C'"));
	if (LOADINGSCREEN_WIDGET.Succeeded()) LodingScreen_Class = LOADINGSCREEN_WIDGET.Class;

}

void UGameInfo_Instance::Init()
{
	if (IOnlineSubsystem* subSystem = IOnlineSubsystem::Get()) {
		sessionInterface = subSystem->GetSessionInterface();
		if (sessionInterface.IsValid()) {
			//SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGameInfo_Instance::OnCreateSessionComplete);
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGameInfo_Instance::OnCreateSessionComplete_Lobby);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInfo_Instance::OnJoinSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGameInfo_Instance::OnFindSessionsComplete);
		}
	}
	sessionSearch.Reset();
	//GetEngine()->OnNetworkFailure().AddUObject(this, &UGameInfo_Instance::HandleNetworkError);
}

void UGameInfo_Instance::OnCreateSessionComplete(FName server_Name, bool succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete: Succeeded %d"), succeeded);
	if (succeeded) {
		GetWorld()->ServerTravel("/Game/Maps/LobyMap.LobyMap?listen");
	}

}

void UGameInfo_Instance::OnCreateSessionComplete_Lobby(FName server_Name, bool succeeded)
{
	
	if (succeeded) {
		UE_LOG(LogTemp, Warning, TEXT("Lobby: OnCreateSessionComplete Succeeded"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("OnCreateSession: Succeeded"));
		//GetWorld()->ServerTravel("/Game/Map/LobyMap.LobyMap?listen");
		UGameplayStatics::OpenLevel(GetWorld(), lobbyName, true, "listen");
	}
	else {  // 세션을 만드는데 실패했을 때
		UE_LOG(LogTemp, Warning, TEXT("Lobby: OnCreateSessionComplete Failed"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("OnCreateSession: Failed"));
	}
}

void UGameInfo_Instance::OnJoinSessionComplete(FName server_Name, EOnJoinSessionCompleteResult::Type type)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("OnJoinSessionComplete"));
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete: Succeeded %d"), type);
	if (type == EOnJoinSessionCompleteResult::Success) {
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete: Succeeded"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete: Failed"));
	}
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		FString JoinAddress = "";
		sessionInterface->GetResolvedConnectString(server_Name, JoinAddress);
		PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UGameInfo_Instance::OnFindSessionsComplete(bool succeeded)
{

	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete: Succeeded %d"), succeeded);
	if (succeeded && sessionSearch->SearchResults.Num() > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("OnFindSessionsSucceeded"));
		sessionsNum = sessionSearch->SearchResults.Num();
		IsFindServer = true;
		//Join_Server();
		findSessionSucceeded.Broadcast();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("OnFindSessionsFailed"));
		IsFindServer = false;
		findSessionFaild.Broadcast();
	}

}

/*void UGameInfo_Instance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	//HandleNetworkError()
}*/

void UGameInfo_Instance::HandleNetworkError(ENetworkFailure::Type FailureType, bool IsServer)
{
	if (!IsServer) {
		UE_LOG(LogTemp, Warning, TEXT("NetError: %s"), *NetErrorToString(FailureType));
	}
}

void UGameInfo_Instance::HandleTravelError(ETravelFailure::Type FailureType)
{
	UE_LOG(LogTemp, Warning, TEXT("TravelError: %s"), *TravelErrorToString(FailureType));
}

void UGameInfo_Instance::CreateServer(int32 Player_Num, FName Server_Name, bool Is_Lan)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("CreateServer"));
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = false;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;
	//SessionSettings.bUsesPresence = false;
	//SessionSettings.bIsDedicated = true;
	sessionSettings.NumPublicConnections = 5;
	sessionInterface->DestroySession(Server_Name);
	sessionInterface->CreateSession(Player_Num, Server_Name, sessionSettings);
}



void UGameInfo_Instance::ShowMainMenu()
{
	auto PlayerContoller = GetFirstLocalPlayerController();
	if (MainMenu_WB == nullptr) {
		MainMenu_WB = CreateWidget<UUserWidget>(PlayerContoller, MainMenu_Class);
	}
	MainMenu_WB->AddToViewport();
	PlayerContoller->bShowMouseCursor = true;
}

void UGameInfo_Instance::ShowHostMenu()
{
	auto PlayerContoller = GetFirstLocalPlayerController();
	if (HostMenu_WB == nullptr) {
		HostMenu_WB = CreateWidget<UUserWidget>(PlayerContoller, HostMenu_Class);
	}
	HostMenu_WB->AddToViewport();
	PlayerContoller->bShowMouseCursor = true;
}

void UGameInfo_Instance::ShowServerMenu()
{
	auto PlayerContoller = GetFirstLocalPlayerController();
	ServerMenu_WB = CreateWidget<UUserWidget>(PlayerContoller, ServerMenu_Class);
	ServerMenu_WB->AddToViewport();
	PlayerContoller->bShowMouseCursor = true;
}

void UGameInfo_Instance::ShowOptionMenu()
{
	auto PlayerContoller = GetFirstLocalPlayerController();
	if (OptionMenu_WB == nullptr) {
		OptionMenu_WB = CreateWidget<UOptionMenu_UserWidget>(PlayerContoller, OptionMenu_Class);
	}
	OptionMenu_WB->AddToViewport();
	PlayerContoller->bShowMouseCursor = true;

	if (!IsCreateSaveFile) {
		OptionMenu_WB->WelcomeMessageVis = ESlateVisibility::Visible;
	}
	else {
		OptionMenu_WB->WelcomeMessageVis = ESlateVisibility::Hidden;
	}
}

void UGameInfo_Instance::LaunchLobby(int32 playerNum, FName server_Name, bool Is_Lan)
{
	maxPlayer = playerNum;
	serverName = server_Name;
	ShowLodingScreen();
	CreateServer(playerNum, server_Name, Is_Lan);
}

void UGameInfo_Instance::JoinServer()
{
	TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;
	FString sessionId = searchResults[0].Session.GetSessionIdStr();
	UE_LOG(LogTemp, Warning, TEXT("SearchResults Count: %s"), *sessionId);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Join Server"));
	//ServerName= SearchResults[0]->
	sessionInterface->JoinSession(0, "test", searchResults[0]);

}

void UGameInfo_Instance::FindServer()
{
	IOnlineSubsystem* onlineSub = IOnlineSubsystem::Get();

	if (onlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = onlineSub->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			sessionSearch = MakeShareable(new FOnlineSessionSearch());
			sessionSearch->bIsLanQuery = false;
			//SessionSearch->dedica
			sessionSearch->MaxSearchResults = 50;
			//SessionSearch->PingBucketSize = 50;
			//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = sessionSearch.ToSharedRef();

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			sessionInterface->FindSessions(0, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}

}

void UGameInfo_Instance::ShowLodingScreen()
{
	auto PlayerContoller = GetFirstLocalPlayerController();
	if (LodingScreen_WB == nullptr) {
		LodingScreen_WB = CreateWidget<UUserWidget>(PlayerContoller, LodingScreen_Class);
	}
	LodingScreen_WB->AddToViewport();
}

void UGameInfo_Instance::Destroy_SessionCaller(APlayerController PC)
{
	// DestroySession() 작성하기
}

void UGameInfo_Instance::Check_SaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(playerSettingsSave, 0)) {
		ShowMainMenu();
		IsCreateSaveFile = true;
	}
	else {
		ShowOptionMenu();
		auto PlayerContoller = GetFirstLocalPlayerController();
		PlayerContoller->bShowMouseCursor = true;
	}
}

FString UGameInfo_Instance::NetErrorToString(ENetworkFailure::Type FailureType)
{
	return FString();
}

FString UGameInfo_Instance::TravelErrorToString(ETravelFailure::Type FailureType)
{
	return FString();
}
