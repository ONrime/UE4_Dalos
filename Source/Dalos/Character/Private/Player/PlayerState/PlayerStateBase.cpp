// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerStateBase.h"

// Sets default values for this component's properties
UPlayerStateBase::UPlayerStateBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerStateBase::StateStart(AMultiPlayerBase* player)
{
}
void UPlayerStateBase::StateUpdate(AMultiPlayerBase* player)
{
}
void UPlayerStateBase::StateEnd(AMultiPlayerBase* player)
{
}

UClass* UPlayerStateBase::GetState()
{
	return nullptr;
}
