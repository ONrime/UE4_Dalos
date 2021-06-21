// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Dalos/Game/Public/CustomStructs.h"
#include "PlayerBody_AnimInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FAnimNotifyDelegate);

UCLASS()
class DALOS_API UPlayerBody_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerBody_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayVaultMontage();
	void PlayClimbMontage();
	void StopMontage();

	FAnimNotifyDelegate vaultDelegate;
	FAnimNotifyDelegate climbDelegate;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	bool IsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	bool IsJumped = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float upperYaw = 0.0f;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float upperPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float playerSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	FVelocityBlend velocityBlend;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float DirForward = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	float DirRight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	UClass* downStateNClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
	UClass* upperStateNClass = nullptr;
	FRotator standingDir = FRotator::ZeroRotator;
	FRotator standingDirEnd = FRotator::ZeroRotator;
	bool IsTurn = false;

	float TurnBodyYaw(class AMultiPlayerBase* player);
	FVelocityBlend GetVeloctyBlend(FVector playerVelocity, FRotator playerRotator, FVelocityBlend& current);

	UAnimMontage* Vault_Montage;
	UAnimMontage* Climb_Montage;

	UFUNCTION()
	void AnimNotify_ClimbEnd();
	UFUNCTION()
	void AnimNotify_VaultEnd();


};
