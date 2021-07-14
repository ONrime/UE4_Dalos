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

	UPROPERTY(Transient)
	FName weaponName = "";
	UPROPERTY(Transient)
	int rifleAmmo = 0;
	UPROPERTY(Transient)
	int pistolAmmo = 0;
	UPROPERTY(Transient)
	int loadedAmmo = 0;
	UPROPERTY(Transient)
	int equipAmmo = 0;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// , ReplicatedUsing = OnRep_UpdatePlayerHP
	UPROPERTY(Transient, Replicated) // 직렬화에서 제회시키는 키워드이다. 데이터가 상시 변하기 때문에 보관하는게 의미가 없다.
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
