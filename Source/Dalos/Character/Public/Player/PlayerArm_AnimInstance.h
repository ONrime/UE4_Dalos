// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerArm_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UPlayerArm_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
		UPlayerArm_AnimInstance();
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float playerSpeed =0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	UClass* upperStateNClass;
	
};
