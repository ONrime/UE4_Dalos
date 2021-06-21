// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerStateBase.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "PlayerDownStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UPlayerDownStateBase : public UPlayerStateBase
{
	GENERATED_BODY()
	
public:
	UPlayerDownStateBase();

	virtual UPlayerDownStateBase* HandleInput(class AMultiPlayerBase* player);
	virtual UPlayerDownStateBase* SendHandleInput(EPlayerPress press);
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

	virtual void TurnAtRate(class AMultiPlayerBase* player, float Rate);
	virtual void LookUpAtRate(class AMultiPlayerBase* player, float Rate);
	virtual UPlayerDownStateBase* PlayerJump(class AMultiPlayerBase* player);
	virtual void PlayerMove(class AMultiPlayerBase* player, float inputValue, float inputDirRight);

protected:
	UPlayerDownStateBase* temp;

private:

};
