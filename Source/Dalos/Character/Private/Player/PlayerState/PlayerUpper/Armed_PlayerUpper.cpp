// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/Armed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/UnArmed_PlayerUpper.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/ADS_PlayerUpper.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Dalos/Weapone/Public/WeaponeBase.h"

UArmed_PlayerUpper::UArmed_PlayerUpper()
{
	PrimaryComponentTick.bCanEverTick = false;
	temp = nullptr;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>ADSCURVE(TEXT("CurveFloat'/Game/Curve/ADSChange.ADSChange'"));
	if (ADSCURVE.Succeeded()) {
		ADSCurve = ADSCURVE.Object;
	}
}

UPlayerUpperStateBase* UArmed_PlayerUpper::HandleInput(AMultiPlayerBase* player)
{
	auto playerController = Cast<APlayerController>(player->GetController());
	auto playerInput = Cast<UPlayerInput>(playerController->PlayerInput);
	TArray <FInputActionKeyMapping> actionUnArmed = playerInput->GetKeysForAction(TEXT("UnArmed"));
	TArray <FInputActionKeyMapping> actionADS = playerInput->GetKeysForAction(TEXT("ADS"));

	if (playerInput->IsPressed(actionUnArmed[0].Key)) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (playerInput->IsPressed(actionADS[0].Key)) {
		temp = NewObject<UADS_PlayerUpper>(this, UADS_PlayerUpper::StaticClass());
	}

	return temp;
}

UPlayerUpperStateBase* UArmed_PlayerUpper::SendHandleInput(EPlayerPress press)
{
	if (press == EPlayerPress::UNARMED) {
		temp = NewObject<UUnArmed_PlayerUpper>(this, UUnArmed_PlayerUpper::StaticClass());
	}
	else if (press == EPlayerPress::ADS) {
		temp = NewObject<UADS_PlayerUpper>(this, UADS_PlayerUpper::StaticClass());
	}
	return temp;
}

void UArmed_PlayerUpper::StateStart(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Armed: StateStart"));

	playerCamera = player->FollowCamera;

	if (ADSCurve != nullptr && player->upperStateBClass == UADS_PlayerUpper::StaticClass()) {
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, "SetUnADS");
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, "SetUnADSFinish");
		ADSTimeline.AddInterpFloat(ADSCurve, TimelineCallback);
		ADSTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		ADSTimeline.SetPlayRate(4.0f);
		ADSTimeline.PlayFromStart();
	}
}

void UArmed_PlayerUpper::StateUpdate(AMultiPlayerBase* player)
{
	ADSTimeline.TickTimeline(GetWorld()->DeltaTimeSeconds);
	if (player->HasAuthority()) {
		player->IsHandUp = HandUpTracer(player);
	}
	
}

void UArmed_PlayerUpper::StateEnd(AMultiPlayerBase* player)
{
	UE_LOG(LogTemp, Warning, TEXT("Armed: StateEnd"));
}

UClass* UArmed_PlayerUpper::GetState()
{
	return UArmed_PlayerUpper::StaticClass();
}

void UArmed_PlayerUpper::PlayerFire(AMultiPlayerBase* player, AWeaponeBase* equip, bool& IsAuto, float& count, FVector loc, FRotator rot, FVector bulletLoc)
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
		player->FireBullet(loc, rot, bulletLoc);
	}
	
}

bool UArmed_PlayerUpper::HandUpTracer(AMultiPlayerBase* player)
{
	TArray<AActor*> actorstoIgnore;
	FHitResult outHit;
	FVector startTrace = player->GetMesh()->GetSocketLocation("HandLoc");
	FVector endTrace = startTrace + player->FollowCamera->GetForwardVector() * 75.0f;
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(this, startTrace, endTrace, 8.0f, ETraceTypeQuery::TraceTypeQuery1
		, false, actorstoIgnore, EDrawDebugTrace::ForOneFrame, outHit, true);

	return IsHit;
}

void UArmed_PlayerUpper::SetUnADS()
{
	if (playerCamera) {
		playerCamera->SetFieldOfView(FMath::Lerp(50.0f, 90.0f, ADSTimeline.GetPlaybackPosition()));
	}
}
void UArmed_PlayerUpper::SetUnADSFinish()
{

}


