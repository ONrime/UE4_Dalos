// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/PlayerUpperStateBase.h"
#include "Armed_PlayerUpper.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UArmed_PlayerUpper : public UPlayerUpperStateBase
{
	GENERATED_BODY()
	
public:
	UArmed_PlayerUpper();

	virtual UPlayerUpperStateBase* HandleInput(class AMultiPlayerBase* player) override;
	virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

protected:

private:

};
