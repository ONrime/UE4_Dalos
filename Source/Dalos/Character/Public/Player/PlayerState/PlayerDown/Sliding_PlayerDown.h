// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/PlayerDownStateBase.h"
#include "Sliding_PlayerDown.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API USliding_PlayerDown : public UPlayerDownStateBase
{
	GENERATED_BODY()
	
public:
	USliding_PlayerDown();

	virtual UPlayerDownStateBase* HandleInput(class AMultiPlayerBase* player) override;
	virtual UPlayerDownStateBase* SendHandleInput(EPlayerPress press) override;
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

private:
	FVector cameraLoc = FVector::ZeroVector;
	float pelvisZ = 20.0f;

	float GetSlidingAngle(FVector loc, FVector dir, FVector& slidingDir);
	float slidingAngle = 0.0f;
	float baseForce = 3.0f;
	FVector forceDir = FVector::ZeroVector;

};
