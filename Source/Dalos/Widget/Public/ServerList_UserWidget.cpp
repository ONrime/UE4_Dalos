// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Public/ServerList_UserWidget.h"

void UServerList_UserWidget::ServerListAdd()
{
	FName sessionName = FName(*(SearchResult.Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString()));
	ServerName = FName(*(SearchResult.Session.OwningUserName)).ToString();
	if (ServerName == "") ServerName = "NoBody";
	PlayerNum = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
	FName gameModeName = FName(*(SearchResult.Session.SessionSettings.Settings.FindRef("GAMEMODE_NAME").Data.ToString()));
	GameModeName = gameModeName.ToString();
	Ping = SearchResult.PingInMs;
}
