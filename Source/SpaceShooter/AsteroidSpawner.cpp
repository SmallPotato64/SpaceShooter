// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"
#include "Asteroid.h"
#include "TimerManager.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, SpawnInterval, true);
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsteroidSpawner::SpawnAsteroid()
{
	if (!AsteroidClass) return;

	// Pick a random angle around a circle outside the screen
	float RandomAngleRad = FMath::RandRange(0.0f, TWO_PI);
	FVector CenterLocation = GetActorLocation();

	float RandomZOffset = FMath::RandRange(-3.0f, 3.0f);
	
	FVector SpawnOffset = FVector(FMath::Cos(RandomAngleRad), FMath::Sin(RandomAngleRad), 0.0f) * SpawnRadius;
	FVector SpawnLocation = CenterLocation + SpawnOffset;
	SpawnLocation.Z += RandomZOffset;
	
	// Choose a random target location near center so it flies across the screen at varying angles
	FVector TargetOffset = FVector(FMath::RandRange(-InnerTargetRadius, InnerTargetRadius), 
								   FMath::RandRange(-InnerTargetRadius, InnerTargetRadius), 0.0f);
	FVector TargetLocation = CenterLocation + TargetOffset;

	// Point the asteroid toward the target location
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn and initialize
	AAsteroid* NewAsteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, SpawnLocation, SpawnRotation, SpawnParams);
}
