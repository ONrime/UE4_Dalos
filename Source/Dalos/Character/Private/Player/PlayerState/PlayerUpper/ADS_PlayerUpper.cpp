// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/ADS_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Dalos/Weapone/Public/WeaponeBase.h"

UADS_PlayerUpper::UADS_PlayerUpper()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>ADSCURVE(TEXT("CurveFloat'/Game/Curve/ADSChange.ADSChange'"));
	if (ADSCURVE.Succeeded()) {
		ADSCurve = ADSCURVE.Object;
	}
}

UPlayerUpperStateBase* UADS_PlayerUpper::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray <FInputActionKeyMapping> actionUnArmed = playerInput->GetKeysForAction(TEXT("UnArmed"));
	TArray <FInputActionKeyMapping> actionADS = playerInput->GetKeysForAction(TEXT("ADS"));

	if (playerInput->IsPressed(actionUnArmed[0].Key)) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (playerInput->InputKey(actionADS[0].Key, EInputEvent::IE_Released, 1.0f, true)) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UADS_PlayerUpper::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::UNARMED) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (press == EPlayerPress::ADS) {
		temp = NewObject<UArmed_PlayerUpper>(this, UArmed_PlayerUpper::StaticClass());
	}
	return temp;
}

void UADS_PlayerUpper::StateStart(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("ADS: StateStart"));

	playerCamera = player->FollowCamera;

	if (ADSCurve != nullptr) {
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, "SetADS");
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, "SetADSFinish");
		ADSTimeline.AddInterpFloat(ADSCurve, TimelineCallback);
		ADSTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		ADSTimeline.SetPlayRate(4.0f);
		ADSTimeline.PlayFromStart();
	}
}

void UADS_PlayerUpper::StateUpdate(AMultiPlayerBase* player)
{
	ADSTimeline.TickTimeline(GetWorld()->DeltaTimeSeconds);
}

void UADS_PlayerUpper::StateEnd(AMultiPlayerBase* player)
{
	//UE_LOG(LogTemp, Warning, TEXT("ADS: StateEnd"));
}

UClass* UADS_PlayerUpper::GetState()
{
	return UADS_PlayerUpper::StaticClass();
}

void UADS_PlayerUpper::PlayerFire(AMultiPlayerBase* player, AWeaponeBase* equip, bool& IsAuto, float& count)
{
	if (equip) {
		if (equip->GetWeaponeLever() == WEAPONLEVER::FULLAUTO) {
			IsAuto = true;
			count = 0.0f;
		}
		else if (equip->GetWeaponeLever() == WEAPONLEVER::SINGLEFIRE) {
			IsAuto = false;
			count = 0.0f;
		}
		else {
			IsAuto = true;
			count = 3.0f;
		}
	}
	player->FireBullet();
}

void UADS_PlayerUpper::SetADS()
{
	//UE_LOG(LogTemp, Warning, TEXT("ADS: SetADS %f"), ADSTimeline.GetPlaybackPosition());
	if (playerCamera) {
		playerCamera->SetFieldOfView(FMath::Lerp(90.0f, 50.0f, ADSTimeline.GetPlaybackPosition()));
	}

}

void UADS_PlayerUpper::SetADSFinish()
{
	//UE_LOG(LogTemp, Warning, TEXT("ADS: SetADSFinish"));
}
