// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/PlayerUpperStateBase.h"

UPlayerUpperStateBase::UPlayerUpperStateBase()
{
	temp = nullptr;
}
UPlayerUpperStateBase* UPlayerUpperStateBase::HandleInput(AMultiPlayerBase* player)
{
	return temp;
}

UPlayerUpperStateBase* UPlayerUpperStateBase::SendHandleInput(EPlayerPress press)
{
	return temp;
}

void UPlayerUpperStateBase::StateStart(AMultiPlayerBase* player)
{
}
void UPlayerUpperStateBase::StateUpdate(AMultiPlayerBase* player)
{
}
void UPlayerUpperStateBase::StateEnd(AMultiPlayerBase* player)
{
}

UClass* UPlayerUpperStateBase::GetState()
{
	return nullptr;
}
