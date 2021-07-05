// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Blueprint/UserWidget.h"

AMultiPlayer_HUD::AMultiPlayer_HUD() 
{
	static ConstructorHelpers::FClassFinder<UUserWidget>CROSSHAIR_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/CrossHair.CrossHair_C'"));
	if (CROSSHAIR_WIDGET.Succeeded()) CrossHairWidget = CreateWidget<UUserWidget>(GetWorld(), CROSSHAIR_WIDGET.Class);
	

}

void AMultiPlayer_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (CrossHairWidget != nullptr) CrossHairWidget->AddToViewport();

}

void AMultiPlayer_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
