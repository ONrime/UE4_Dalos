// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DALOS_API UPlayerStateBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateBase();
	//virtual UPlayerStateBase* HandleInput(class AMultiPlayerBase* player);
	virtual void StateStart(class AMultiPlayerBase* player);
	virtual void StateUpdate(class AMultiPlayerBase* player);
	virtual void StateEnd(class AMultiPlayerBase* player);
	virtual UClass* GetState();
		
};
