// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Dalos/Widget/Public/CrossHair_UserWidget.h"
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
void AMultiPlayer_HUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//CrossHairHideCheck.BindUFunction(this, FName("HideCrossHair"));
	CrossHairHideCheck.BindLambda([this](bool check)->void {
		UCrossHair_UserWidget* cross = Cast<UCrossHair_UserWidget>(CrossHairWidget);
		if (check) {
			cross->corssHairVis = ESlateVisibility::Hidden;
		}else { cross->corssHairVis = ESlateVisibility::Visible; }
		});
	CrossHairRedCheck.BindLambda([this](bool check)->void {
		UCrossHair_UserWidget* cross = Cast<UCrossHair_UserWidget>(CrossHairWidget);
		IsRed = check;
		cross->IsRed = check;
		});
}
void AMultiPlayer_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMultiPlayer_HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	//if (CrossHairHideCheck.IsBound()) CrossHairHideCheck.Unbi
}

void AMultiPlayer_HUD::SetCrossHairSpread(float corss)
{
	UCrossHair_UserWidget* cross = Cast<UCrossHair_UserWidget>(CrossHairWidget);
	cross->crossHairSpread = corss;
}
