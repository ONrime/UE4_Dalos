// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Loby_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API ALoby_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALoby_GameModeBase();

	UFUNCTION(Server, Reliable, WithValidation)
	void SwapCharacter(class APlayerController* controller, class ACharacter* character, bool changedStatus);
	bool SwapCharacter_Validate(class APlayerController* controller, class ACharacter* character, bool changedStatus);
	void SwapCharacter_Implementation(class APlayerController* controller, class ACharacter* character, bool changedStatus);

	UFUNCTION(Server, Reliable, WithValidation)
	void EveryoneUpdate();
	bool EveryoneUpdate_Validate();
	void EveryoneUpdate_Implementation();

protected:

private:
	

};
