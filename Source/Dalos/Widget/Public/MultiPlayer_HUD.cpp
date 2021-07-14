// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Widget/Public/MultiPlayer_HUD.h"
#include "Dalos/Widget/Public/CrossHair_UserWidget.h"
#include "Dalos/Widget/Public/HitCheck_UserWidget.h"
#include "Dalos/Widget/Public/PlayerHitCheck_UserWidget.h"
#include "Dalos/Widget/Public/Ammo_UserWidget.h"
#include "Blueprint/UserWidget.h"

AMultiPlayer_HUD::AMultiPlayer_HUD() 
{
	static ConstructorHelpers::FClassFinder<UUserWidget>CROSSHAIR_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/CrossHair.CrossHair_C'"));
	if (CROSSHAIR_WIDGET.Succeeded()) CrossHairWidget = CreateWidget<UUserWidget>(GetWorld(), CROSSHAIR_WIDGET.Class);
	static ConstructorHelpers::FClassFinder<UUserWidget>HITCHECK_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/HitCheck.HitCheck_C'"));
	if (HITCHECK_WIDGET.Succeeded()) HitCheckClass= HITCHECK_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget>PLAYERHITCHECK_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/PlayerHitCheck.PlayerHitCheck_C'"));
	if (PLAYERHITCHECK_WIDGET.Succeeded()) PlayerHitCheckClass = PLAYERHITCHECK_WIDGET.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget>AMMO_WIDGET(TEXT("WidgetBlueprint'/Game/UI/Player/Ammo.Ammo_C'"));
	if (AMMO_WIDGET.Succeeded()) AmmoWidget = CreateWidget<UUserWidget>(GetWorld(), AMMO_WIDGET.Class);

}

void AMultiPlayer_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (CrossHairWidget != nullptr) CrossHairWidget->AddToViewport();
	if (AmmoWidget != nullptr) AmmoWidget->AddToViewport();

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
	HitRedCheck.BindLambda([this](bool check)->void {
		auto HitCheckWidget = CreateWidget<UUserWidget>(GetWorld(), HitCheckClass);
		if (HitCheckWidget != nullptr) {
			HitCheckWidget->AddToViewport();
			UHitCheck_UserWidget* hit = Cast<UHitCheck_UserWidget>(HitCheckWidget);
			hit->RedCheck(check);
		}
		});
	PlyaerHitLocCheck.BindLambda([this](FVector loc)->void {
		auto PlayerHitCheckWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHitCheckClass);
		if (PlayerHitCheckWidget != nullptr) {
			PlayerHitCheckWidget->AddToViewport();
			UPlayerHitCheck_UserWidget* playerhit = Cast<UPlayerHitCheck_UserWidget>(PlayerHitCheckWidget);
			playerhit->HitLocCheck(loc);
		}
		});
	LoadedAmmoCheck.BindLambda([this](float check)->void {
		UAmmo_UserWidget* ammo = Cast<UAmmo_UserWidget>(AmmoWidget);
		if (ammo) ammo->loadedAmmo = check;
		});
	EquipAmmoCheck.BindLambda([this](float check)->void {
		UAmmo_UserWidget* ammo = Cast<UAmmo_UserWidget>(AmmoWidget);
		if (ammo) ammo->equipAmmo = check;
		});
	WeaponNameCheck.BindLambda([this](FName check)->void {
		UAmmo_UserWidget* ammo = Cast<UAmmo_UserWidget>(AmmoWidget);
		if (ammo) ammo->weaponName = check;
		});
}
void AMultiPlayer_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float speed = 30.0f;
	float target = targetSpread;
	if (IsFire && currentSpread >= targetSpread - 5.0f) { // 한 번 작동
		IsBackSpread = true;
	}
	if (IsBackSpread) {
		speed = 500.0f;
		target = targetSpread - 80.0f;
	}
	currentSpread = FMath::FInterpTo(currentSpread, target, DeltaTime, speed);
	
	//UE_LOG(LogTemp, Warning, TEXT("currentSpread: %f"), currentSpread);
	//UE_LOG(LogTemp, Warning, TEXT("targetSpread: %f"), targetSpread);
	UCrossHair_UserWidget* crossHair = Cast<UCrossHair_UserWidget>(CrossHairWidget);
	crossHair->crossHairSpread = currentSpread;

	UAmmo_UserWidget* ammo = Cast<UAmmo_UserWidget>(AmmoWidget);
	//ammo->loadedAmmo=

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
