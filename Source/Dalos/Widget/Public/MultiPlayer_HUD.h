// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiPlayer_HUD.generated.h"

DECLARE_DELEGATE_OneParam(FDele_HUDCheck, bool check);
DECLARE_DELEGATE_OneParam(FDele_HUDLocCheck, FVector check);
DECLARE_DELEGATE_OneParam(FDele_HUDFloatCheck, float check);
DECLARE_DELEGATE_OneParam(FDele_HUDNameCheck, FName check);

/**
 * 
 */
UCLASS()
class DALOS_API AMultiPlayer_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMultiPlayer_HUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HUD)
	class UUserWidget* CrossHairWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	class UUserWidget* AmmoWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	bool IsRed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	float crossHairSpread = 0.0f;
	/* 승리 카운트, 남은 인원 수 등 */

	FDele_HUDCheck CrossHairHideCheck;
	FDele_HUDCheck CrossHairRedCheck;
	FDele_HUDCheck HitRedCheck;
	FDele_HUDLocCheck PlyaerHitLocCheck;
	FDele_HUDFloatCheck LoadedAmmoCheck;
	FDele_HUDFloatCheck EquipAmmoCheck;
	FDele_HUDNameCheck WeaponNameCheck;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	float currentSpread = 0.0f;
	float targetSpread = 0.0f;
	float backSpread = 0.0f;
	bool IsFire = false;
	bool IsBackSpread = false;

	TSubclassOf<class UUserWidget> HitCheckClass;
	TSubclassOf<class UUserWidget> PlayerHitCheckClass;

public:
	void SetCrossHairSpread(float cross, bool back, bool fire);
	float GetTargetSpread() { return targetSpread; }


};
