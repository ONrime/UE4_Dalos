// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/PlayerDownStateBase.h"
#include "Standing_PlayerDown.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UStanding_PlayerDown : public UPlayerDownStateBase
{
	GENERATED_BODY()
	
public:
	UStanding_PlayerDown();

	virtual UPlayerDownStateBase* HandleInput(class AMultiPlayerBase* player) override;
	virtual UPlayerDownStateBase* SendHandleInput(EPlayerPress press) override;
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

	virtual void TurnAtRate(class AMultiPlayerBase* player, float Rate) override;
	virtual void PlayerMove(class AMultiPlayerBase* player, float inputValue, float inputDirRight) override;

protected:

private:
	FVector cameraLoc = FVector::ZeroVector;
	FRotator cameraRot = FRotator::ZeroRotator;

	float pitch = 0.0f;
	float pelvisZ = 0.0f;
	float cameraMoveChagneRoll = 0.0f;
	float cameraTurnChagneRoll = 0.0f;

	class UCameraShakeBase* walkShake;

};
