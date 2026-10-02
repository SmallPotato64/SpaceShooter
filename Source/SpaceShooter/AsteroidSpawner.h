// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

class AAsteroid;

UCLASS()
class SPACESHOOTER_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AAsteroid> AsteroidClass;

	// Time in seconds between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnInterval = 2.0f;

	// Distance from the center where asteroids spawn (outside screen view)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnRadius = 1000.0f;

	// Radius of target offset area near the center to vary incoming angles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float InnerTargetRadius = 400.0f;
	
	FTimerHandle SpawnTimerHandle;

	void SpawnAsteroid();
};
