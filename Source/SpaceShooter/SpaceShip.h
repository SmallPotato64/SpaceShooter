// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "SpaceShip.generated.h"

class USphereComponent;
class UPaperFlipbookComponent;
class UPaperZDAnimationComponent;
class UFloatingPawnMovement;
class USceneComponent;
class UInputAction;
class UInputMappingContext;


UCLASS()
class SPACESHOOTER_API ASpaceShip : public APawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> Sprite;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* Hitbox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> MuzzlePoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperZDAnimationComponent> AnimComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	float AcceptanceRadius = 50.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class ARocket> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 0.15f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 Life = 3;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsShooting = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMoving = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsDestroyed = false;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> AttackSound;
	
	bool GetIsShooting();
	bool GetIsMoving();
	bool GetIsDestroyed();
	
	FTimerHandle FireTimerHandle;

	void StartFire();
	void StopFire();
	void SpawnProjectile();
	
	
};
