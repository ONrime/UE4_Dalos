// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ammo_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALOS_API UAmmo_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName weaponName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int loadedAmmo = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int equipAmmo = 0;
};
