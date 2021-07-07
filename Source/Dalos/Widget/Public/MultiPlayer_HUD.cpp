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
	/*float backSpread = 0.0f;
	if (IsFire && currentSpread >= targetSpread - 10.0f) {
		backSpread = 10.0f;
	}*/
	float speed = 30.0f;
	float target = targetSpread;
	if (IsFire && currentSpread >= targetSpread - 5.0f) { // 한 번 작동
		//backSpread = 10.0f;
		IsBackSpread = true;
		//UE_LOG(LogTemp, Warning, TEXT("IsBackSpread"));
	}
	if (IsBackSpread) {
		//currentSpread = FMath::FInterpTo(currentSpread, targetSpread - 30.0f, DeltaTime, 80.0f);
		speed = 80.0f;
		target = targetSpread - 30.0f;
		UE_LOG(LogTemp, Warning, TEXT("IsBackSpread"));
	}
	else {
		//currentSpread = FMath::FInterpTo(currentSpread, targetSpread, DeltaTime, 30.0f);
		UE_LOG(LogTemp, Warning, TEXT("NotIsBackSpread"));
	}
	currentSpread = FMath::FInterpTo(currentSpread, target, DeltaTime, speed);
	
	UE_LOG(LogTemp, Warning, TEXT("currentSpread: %f"), currentSpread);
	UE_LOG(LogTemp, Warning, TEXT("targetSpread: %f"), targetSpread);
	UCrossHair_UserWidget* crossHair = Cast<UCrossHair_UserWidget>(CrossHairWidget);
	crossHair->crossHairSpread = currentSpread;

}
void AMultiPlayer_HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	//if (CrossHairHideCheck.IsBound()) CrossHairHideCheck.Unbi
}

void AMultiPlayer_HUD::SetCrossHairSpread(float cross, bool back, bool fire)
{
	IsFire = fire;
	IsBackSpread = back;
	targetSpread = cross;
}
