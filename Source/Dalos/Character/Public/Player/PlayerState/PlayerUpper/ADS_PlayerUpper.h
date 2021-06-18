// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/PlayerUpperStateBase.h"
#include "Components/TimelineComponent.h"
#include "ADS_PlayerUpper.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UADS_PlayerUpper : public UPlayerUpperStateBase
{
	GENERATED_BODY()
	
public:
	UADS_PlayerUpper();

	virtual UPlayerUpperStateBase* HandleInput(class AMultiPlayerBase* player) override;
	virtual UPlayerUpperStateBase* SendHandleInput(EPlayerPress press) override;
	virtual void StateStart(class AMultiPlayerBase* player) override;
	virtual void StateUpdate(class AMultiPlayerBase* player) override;
	virtual void StateEnd(class AMultiPlayerBase* player) override;
	virtual UClass* GetState() override;

	virtual void PlayerFire(class AMultiPlayerBase* player, class AWeaponeBase* equip, bool& IsAuto, float& count) override;

protected:

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* ADSCurve;
	FTimeline ADSTimeline;
	UFUNCTION()
	void SetADS();
	UFUNCTION()
	void SetADSFinish();

	class UCameraComponent* playerCamera;
};
