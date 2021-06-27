// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerArm_AnimInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FPlayerMontage);

UCLASS()
class DALOS_API UPlayerArm_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerArm_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	FPlayerMontage playFire;

	void PlayFireMontage();
	void StopFireMontage();
	void PlayReloadMontage();
	void StopReloadMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	bool IsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	bool IsJumped = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	bool IsHandUp = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float upperPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float playerSpeed =0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	UClass* upperStateNClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	UClass* downStateNClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float inputDir = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float dirForward = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float dirRight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float aimDirRight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float aimDirUp = 0.0f;

	int fireRandNum = 0;

	UAnimMontage* AK_Fire1_Montage;
	UAnimMontage* AK_Fire2_Montage;
	UAnimMontage* AK_Fire_Montage;
	UAnimMontage* AK_ADS_Fire_Montage;
	UAnimMontage* AK_Reload_Montage;
	
};
