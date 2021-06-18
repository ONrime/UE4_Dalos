// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Dalos/Interface/Public/Interaction_Interface.h"
#include "WeaponeBase.generated.h"

UENUM(BlueprintType)
enum class WEAPONSTATE : uint8 {
	EQUIP,
	BACKEQUIP,
	DROP,
	THROW
};

UENUM(BlueprintType)
enum class WEAPONLEVER : uint8 {
	FULLAUTO,
	TRIPLE,
	SINGLEFIRE
};

UCLASS()
class DALOS_API AWeaponeBase : public AActor, public IInteraction_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
	class USkeletalMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InteractionUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float InAmmo = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WEAPONSTATE WeaponeState;
	WEAPONLEVER WeaponeLever;
	bool IsEmpty = false;
	bool IsReload = false;
	float fireRecoilPitch = 0.0f;
	float fireRecoilYaw = 0.0f;

	virtual void StateStart(WEAPONSTATE state);

public:	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interation")
	void InteractionStart();
	virtual void InteractionStart_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interation")
	void InteractionUpdate();
	virtual void InteractionUpdate_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interation")
	void InteractionEnd();
	virtual void InteractionEnd_Implementation() override;

	WEAPONSTATE GetWeaponeState() { return WeaponeState; }; void SetWeaponeState(WEAPONSTATE set);
	WEAPONLEVER GetWeaponeLever() { return WeaponeLever; }; void SetWeaponeLever(WEAPONLEVER set) { WeaponeLever = set; };
	bool GetIsEmpty() { return IsEmpty; }; void SetIsEmpty(bool set) { IsEmpty = set; };
	bool GetIsReload() { return IsReload; }; void SetIsReload(bool set) { IsReload = set; };
	virtual float GetFireRecoilPitch(); 
	virtual float GetFireRecoilYaw();
	virtual UClass* GetStaticClass();
	virtual AWeaponeBase* SpawnToHand(AActor* owner, FVector loc, FRotator rot);

	virtual void ProjectileFire(FVector loc, FRotator rot, FRotator bulletRot);

};
