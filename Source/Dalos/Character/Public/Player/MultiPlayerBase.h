// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Dalos/Character/Public/CharacterBase.h"
#include "MultiPlayerBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerPress : uint8 {
	CROUCH UMETA(DisplayName = "Crouch"),
	SPLINT UMETA(DisplayName = "Splint"),
	ADS UMETA(DisplayName = "ADS"),
	FIRSTGUN UMETA(DisplayName = "FirstGun"),
	SECONDGUN UMETA(DisplayName = "SecondGun"),
	UNARMED UMETA(DisplayName = "UnArmed"),
	INTERACTION UMETA(DisplayName = "Interaction")
	
};

UCLASS()
class DALOS_API AMultiPlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AMultiPlayerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AloowPrivateAccess = "true"))
	class USkeletalMeshComponent* ArmMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AloowPrivateAccess = "true"))
	class USkeletalMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerBase)
	float PlayerSpeed = 70.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	class AWeaponeBase* equipWeapone;
	class AWeaponeBase* backWeapone1;
	class AWeaponeBase* backWeapone2;
	class AWeaponeBase* lookWeapone;
	bool InteractionCheck();
	void WeaponCheck(AWeaponeBase* check);  // 현제 들고 있는 무기들을 확인해서 장착할지 버릴지 결정한다.
	void EquipGunOnHand(AWeaponeBase* equip);
	UClass* equipWeaponeStaticClass = nullptr;

	UPROPERTY(Replicated)
	float upperPitch = 0.0f;
	float upperYaw = 0.0f;
	float upperYaw2 = 0.0f; // 하체고정시 상체 회전 값 yaw2는 90도 정제 없이 180도 기준으로 출력
	//UPROPERTY(Replicated)
	FRotator controllerRot = FRotator::ZeroRotator;
	float aimDirRight = 0.0f, aimDirUp = 0.0f; // 컨트롤러로 부터 받은 시점 키값 (AnimBP에서 사용하기 위해)
	float inputDirForward = 0.0f;
	float inputDirRight = 0.0f; // 컨트롤러로 부터 받은 이동 키값 (AnimBP에서 사용하기 위해)
	FVector moveDir = FVector::ZeroVector;
	float inputDir = 0.0f;
	UPROPERTY(EditAnywhere)
	EPlayerPress playerPress;
	FVector cameraLoc = FVector::ZeroVector;

private:
	// 상태
	class UPlayerUpperStateBase* upperState = nullptr;
	class UPlayerDownStateBase* downState = nullptr;

	void PlayerCrouch();
	void PlayerJump();
	void PlayerSplint();
	void PlayerFirstGun();
	void PlayerSecondGun();
	void PlayerUnArmed();
	void PlayerInteraction();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void PlayerMove();


public:
	UPlayerUpperStateBase* GetUpperState() { return upperState; };
	UPlayerDownStateBase* GetDownState() { return downState; };
	UClass* upperStateBClass = nullptr; UClass* downStateBClass = nullptr;   //전 상태에 대한 Class값 (AnimBP에서 사용하기 위해)
	UClass* upperStateNClass = nullptr; UClass* downStateNClass = nullptr;   //전 상태에 대한 Class값 (AnimBP에서 사용하기 위해)
	void UpperPress(class UPlayerUpperStateBase* state);
	void DownPress(class UPlayerDownStateBase* state);
	float GetUpperPitch() { return upperPitch; }
	float GetUpperYaw() { return upperYaw; }
	void SetUpperYaw(float yaw) { upperYaw = yaw; }
	float GetUpperYaw2() { return upperYaw2; }
	FRotator GetControllerRot() { return controllerRot; }
	float GetAimDirRight() { return aimDirRight; } float GetAimDirUp() { return aimDirUp; }
	float GetInputDirForward() { return inputDirForward; } float GetInputDirRight() { return inputDirRight; }
	EPlayerPress GetPlayerPress() { return playerPress; }

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SendControllerRot(FRotator rot);
	bool SendControllerRot_Validate(FRotator rot);
	void SendControllerRot_Implementation(FRotator rot);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendDownPress(EPlayerPress press);
	bool Server_SendDownPress_Validate(EPlayerPress press);
	void Server_SendDownPress_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendDownPress(EPlayerPress press);
	bool NetMulticast_SendDownPress_Validate(EPlayerPress press);
	void NetMulticast_SendDownPress_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendUpperPress(EPlayerPress press);
	bool Server_SendUpperPress_Validate(EPlayerPress press);
	void Server_SendUpperPress_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendUpperPress(EPlayerPress press);
	bool NetMulticast_SendUpperPress_Validate(EPlayerPress press);
	void NetMulticast_SendUpperPress_Implementation(EPlayerPress press);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendWeaponeCheck(AWeaponeBase* check);
	bool Server_SendWeaponeCheck_Validate(AWeaponeBase* check);
	void Server_SendWeaponeCheck_Implementation(AWeaponeBase* check);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWeaponeCheck(AWeaponeBase* check);
	bool NetMulticast_SendWeaponeCheck_Validate(AWeaponeBase* check);
	void NetMulticast_SendWeaponeCheck_Implementation(AWeaponeBase* check);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendWeaponeChange(EPlayerPress press);
	bool Server_SendWeaponeChange_Validate(EPlayerPress press);
	void Server_SendWeaponeChange_Implementation(EPlayerPress press);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendWeaponeChange(EPlayerPress press);
	bool NetMulticast_SendWeaponeChange_Validate(EPlayerPress press);
	void NetMulticast_SendWeaponeChange_Implementation(EPlayerPress press);

	bool IsMove = true;
	bool IsPlayerCameraTurn = true;
	bool IsPlayerRotationYawSpeedSlow = false;

};
