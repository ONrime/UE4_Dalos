// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Public/FindServer_UserWidget.h"
#include "Dalos/Widget/Public/ServerList_UserWidget.h"
#include "Dalos/Game/Public/GameInfo_Instance.h"

void UFindServer_UserWidget::FindServerSucceeded()
{
	UGameInfo_Instance* Ins = Cast<UGameInfo_Instance>(GetGameInstance());
	if (Ins && Ins->Results.Num() > 0) {
		Results = Ins->Results;
	}
	else {

	}
}

void UFindServer_UserWidget::AddFindServerList(UServerList_UserWidget* widget, int i)
{
	if (widget) {
		widget->SearchResult = Results[i];
		widget->SelectNum = i;
	}
}
