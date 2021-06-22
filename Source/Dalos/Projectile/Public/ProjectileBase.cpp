// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Projectile/Public/ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//SetReplicates(true);
	//SetReplicateMovement(true);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESHBODY"));
	RootComponent = BodyMesh;
	BodyMesh->SetRelativeScale3D(FVector(2.0f, 0.025f, 0.025f));
	BodyMesh->SetCollisionProfileName("Projectile");
	BodyMesh->SetGenerateOverlapEvents(true);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOLLISION"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(2.5f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MOVEMENT"));
	Movement->SetUpdatedComponent(RootComponent);
	Movement->InitialSpeed = 1800.0f;
	Movement->MaxSpeed = 100000.0f;
	Movement->bRotationFollowsVelocity = false;
	Movement->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectileBase::ProjectileFire(FVector FireDir)
{
	UE_LOG(LogTemp, Warning, TEXT("ProjectileFire"));
	Movement->Velocity = FireDir * Movement->InitialSpeed;
}
void AProjectileBase::SetProjectileVelocity(float Velocity)
{
	Movement->InitialSpeed = Velocity;
}

