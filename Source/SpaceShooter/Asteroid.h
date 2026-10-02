// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class USphereComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class UProjectileMovementComponent;

UCLASS()
class SPACESHOOTER_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Hitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> Sprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	TObjectPtr<UPaperFlipbook> DestroyedSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 400.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinSpeed = 200.0f;
	
	// Preset Tier (1 = Smallest, 4 = Largest)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Stats")
	int32 SizeTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Stats")
	int32 Health = 1;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> SpaceShipHitSound;
	
	float RotationSpeed = 0.0f;
	
	void ApplyAsteroidDamage(int32 Amount);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);
};
