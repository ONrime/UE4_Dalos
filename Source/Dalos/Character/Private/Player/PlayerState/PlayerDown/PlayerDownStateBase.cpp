// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/PlayerDownStateBase.h"

UPlayerDownStateBase::UPlayerDownStateBase()
{
	temp = nullptr;
}
UPlayerDownStateBase* UPlayerDownStateBase::HandleInput(AMultiPlayerBase* player)
{
	return temp;
}

UPlayerDownStateBase* UPlayerDownStateBase::SendHandleInput(EPlayerPress press)
{
	return temp;
}

void UPlayerDownStateBase::StateStart(AMultiPlayerBase* player)
{
}
void UPlayerDownStateBase::StateUpdate(AMultiPlayerBase* player)
{
}
void UPlayerDownStateBase::StateEnd(AMultiPlayerBase* player)
{
}

UClass* UPlayerDownStateBase::GetState()
{
	return nullptr;
}

void UPlayerDownStateBase::TurnAtRate(AMultiPlayerBase* player, float Rate)
{
	if (player->IsPlayerCameraTurn) {
		player->SpringArm->bUsePawnControlRotation = true;
		float PitchRate = Rate * player->BaseTurnRate * 0.008f;
		player->AddControllerYawInput(PitchRate);
	}
	else { // 카메라 움직임 끄기
		//player->SpringArm->bUsePawnControlRotation = false;
	}
}

void UPlayerDownStateBase::LookUpAtRate(AMultiPlayerBase* player, float Rate)
{
	if (player->IsPlayerCameraTurn) {
		player->SpringArm->bUsePawnControlRotation = true;
		float PitchRate = Rate * player->BaseLookUpRate * 0.008f;
		player->AddControllerPitchInput(PitchRate);
	}
	else { // 카메라 움직임 끄기
		//player->SpringArm->bUsePawnControlRotation = false;
	}
}

UPlayerDownStateBase* UPlayerDownStateBase::PlayerJump(AMultiPlayerBase* player)
{
	player->bPressedJump = true;
	player->JumpKeyHoldTime = 0.0f;
	return nullptr;
}

void UPlayerDownStateBase::PlayerMove(AMultiPlayerBase* player, float inputValue, float inputDirRight)
{

}
