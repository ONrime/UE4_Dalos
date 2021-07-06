// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiPlayer_HUD.generated.h"

DECLARE_DELEGATE_OneParam(FDele_HUDCheck, bool check);

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

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void SetCrossHairSpread(float corss);


};
