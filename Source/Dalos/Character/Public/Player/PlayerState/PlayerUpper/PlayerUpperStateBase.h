// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerStateBase.h"
#include "Dalos/Character/Public/Player/MultiPlayerBase.h"
#include "PlayerUpperStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UPlayerUpperStateBase : public UPlayerStateBase
{
	GENERATED_BODY()
	
public:
	UPlayerUpperStateBase();

	virtual UPlayerUpperStateBase* HandleInput(class AMultiPlayerBase* player);
	virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press);
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

	virtual void PlayerFire(class AMultiPlayerBase* player, class AWeaponeBase* equip, bool& IsAuto, float& count);

protected:
	UPlayerUpperStateBase* temp;

private:

	
};
