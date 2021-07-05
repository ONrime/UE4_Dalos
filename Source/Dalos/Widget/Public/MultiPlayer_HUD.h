// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiPlayer_HUD.generated.h"

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
	/* �¸� ī��Ʈ, ���� �ο� �� �� */

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
