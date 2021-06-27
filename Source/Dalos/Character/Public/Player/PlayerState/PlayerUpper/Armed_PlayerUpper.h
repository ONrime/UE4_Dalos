// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dalos/Character/Public/Player/PlayerState/PlayerUpper/PlayerUpperStateBase.h"
#include "Components/TimelineComponent.h"
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

	virtual void PlayerFire(class AMultiPlayerBase* player, class AWeaponeBase* equip, bool& IsAuto, float& count, FVector loc, FRotator rot, FRotator bulletRotation) override;
	virtual bool HandUpTracer(class AMultiPlayerBase* player) override;

protected:

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* ADSCurve;
	FTimeline ADSTimeline;
	UFUNCTION()
	void SetUnADS();
	UFUNCTION()
	void SetUnADSFinish();

	class UCameraComponent* playerCamera;

};
