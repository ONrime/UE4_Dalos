// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerDown/PlayerDownStateBase.h"
#include "Crouch_PlayerDown.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UCrouch_PlayerDown : public UPlayerDownStateBase
{
	GENERATED_BODY()
	
public:
	UCrouch_PlayerDown();

	virtual UPlayerDownStateBase* HandleInput(class AMultiPlayerBase* player) override;
	virtual UPlayerDownStateBase* SendHandleInput(EPlayerPress press) override;
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

	virtual UPlayerDownStateBase* PlayerJump(class AMultiPlayerBase* player) override;

protected:

private:
	FVector cameraLoc = FVector::ZeroVector;

};
