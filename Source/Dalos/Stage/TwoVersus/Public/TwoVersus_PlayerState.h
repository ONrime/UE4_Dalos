// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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
	int GetPlyaerHP() { return playerHP; }
	int GetPlayerXP() { return playerXP; }

protected:
	UPROPERTY(Transient) // 직렬화에서 제회시키는 키워드이다. 데이터가 상시 변하기 때문에 보관하는게 의미가 없다.
	int playerHP = 100;
	UPROPERTY(Transient)
	int playerXP = 0;

private:

public:

};
