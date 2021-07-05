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
	UPROPERTY(Transient) // ����ȭ���� ��ȸ��Ű�� Ű�����̴�. �����Ͱ� ��� ���ϱ� ������ �����ϴ°� �ǹ̰� ����.
	int playerHP = 100;
	UPROPERTY(Transient)
	int playerXP = 0;

private:

public:

};
