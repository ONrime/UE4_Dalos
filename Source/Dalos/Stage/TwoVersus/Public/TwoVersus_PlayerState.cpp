// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "Dalos/Stage/TwoVersus/Public/TwoVersus_GameMode.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

ATwoVersus_PlayerState::ATwoVersus_PlayerState() 
{
	PrimaryActorTick.bCanEverTick = true;
	playerHP = 100.0f;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>HEAL_CURVE(TEXT("CurveFloat'/Game/Curve/HealChange.HealChange'"));
	if (HEAL_CURVE.Succeeded()) healCurve = HEAL_CURVE.Object;
}

void ATwoVersus_PlayerState::DamageToHP(float damage)
{
	playerHP = playerHP - damage;
}

void ATwoVersus_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	

}
void ATwoVersus_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ATwoVersus_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	healTimeline.TickTimeline(DeltaTime);
	//if(HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("playerHP1: %f"), playerHP);
}

void ATwoVersus_PlayerState::StartHeal()
{
	if (healCurve != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("StartHeal"));
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, "SetHeal");
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, "SetHealFinish");
		healTimeline.AddInterpFloat(healCurve, TimelineCallback);
		healTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		healTimeline.SetPlayRate(0.05f);
		healTimeline.PlayFromStart();
	}
}
void ATwoVersus_PlayerState::StopHeal()
{
	healTimeline.Stop();
}

void ATwoVersus_PlayerState::SetHeal()
{
	if (playerHP < 90.0f) {
		playerHP = FMath::Lerp(playerHP, 90.0f, healTimeline.GetPlaybackPosition());
		//UE_LOG(LogTemp, Warning, TEXT("SetHeal_healTimeline: %f"), healTimeline.GetPlaybackPosition());
		//UE_LOG(LogTemp, Warning, TEXT("SetHeal_playerHP: %f"), playerHP);
	}
}
void ATwoVersus_PlayerState::SetHealFinish()
{
	//UE_LOG(LogTemp, Warning, TEXT("HealFinish: %f"), playerHP);
}

void ATwoVersus_PlayerState::OnRep_UpdatePlayerHP()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdatePlayerHP"));
	player = Cast<AMultiPlayerBase>(GetPawn());
	if (player) player->CheckPlayerHP(playerHP);
}

void ATwoVersus_PlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATwoVersus_PlayerState, playerHP);

}
