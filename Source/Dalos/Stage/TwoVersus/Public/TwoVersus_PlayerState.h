// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Components/TimelineComponent.h"
#include "TwoVersus_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ATwoVersus_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATwoVersus_PlayerState();

	void DamageToHP(float damage);
	float GetPlyaerHP() { return playerHP; }
	int GetPlayerXP() { return playerXP; }
	void StartHeal();
	void StopHeal();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_UpdatePlayerHP) // ����ȭ���� ��ȸ��Ű�� Ű�����̴�. �����Ͱ� ��� ���ϱ� ������ �����ϴ°� �ǹ̰� ����.
	float playerHP = 100;
	UPROPERTY(Transient)
	int playerXP = 0;
	UPROPERTY()
	class AMultiPlayerBase* player = nullptr;

	FTimerHandle healTimer;
	UPROPERTY(EditAnywhere)
	UCurveFloat* healCurve;
	FTimeline healTimeline;
	UFUNCTION()
	void SetHeal();
	UFUNCTION()
	void SetHealFinish();
	UFUNCTION()
	void OnRep_UpdatePlayerHP();

private:

public:

};
