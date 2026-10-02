// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShip.h"
#include "Rocket.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShooterGameModeBase.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(Hitbox);
	Hitbox->SetSphereRadius(32.0f);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hitbox->SetGenerateOverlapEvents(true);
	
	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipFlipbook"));
	Sprite->SetupAttachment(RootComponent);
	
	Sprite->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	AnimComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("PaperZDAnimationComponent"));
	
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	MuzzlePoint->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 800.0f;
	MovementComponent->Acceleration = 2000.0f;
	MovementComponent->Deceleration = 1000.0f;
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FInputModeGameAndUI InputMode;
		
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		
		InputMode.SetHideCursorDuringCapture(false);
		
		PC->bShowMouseCursor = true;
		
		PC->SetInputMode(InputMode);
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		if (bIsDestroyed)
		{
			return;
		}
		
		FVector MouseWorldLocation, MouseWorldDirection;
        
		// Get the mouse position and the direction the camera is looking
		if (PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
		{
			// Mathematically project that line down to the spaceship's Z-height
			float DistanceToShipPlane = (GetActorLocation().Z - MouseWorldLocation.Z) / MouseWorldDirection.Z;
			FVector TargetLocation = MouseWorldLocation + (MouseWorldDirection * DistanceToShipPlane);
            
			// Calculate distance and direction to the target
			FVector CurrentLocation = GetActorLocation();
			FVector Direction = (TargetLocation - CurrentLocation);
			Direction.Z = 0.0f; // Force strict 2D math
            
			float DistanceToMouse = Direction.Size();
            
			// Move and rotate ONLY if the cursor is far enough away
			if (DistanceToMouse > AcceptanceRadius) 
			{
				bIsMoving = true;
				Direction.Normalize();
                
				// Rotate the ship to face the cursor
				SetActorRotation(Direction.Rotation());
                
				// Push the ship forward. Your UFloatingPawnMovement handles the MaxSpeed and Acceleration.
				AddMovementInput(Direction, 1.0f);
			}
			else
			{
				bIsMoving = false;
			}
		}
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Start shooting when the button is pressed
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASpaceShip::StartFire);
        
		// Stop shooting when the button is released
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASpaceShip::StopFire);
	}

}

bool ASpaceShip::GetIsShooting()
{
	return bIsShooting; 
}

bool ASpaceShip::GetIsMoving()
{
	return bIsMoving; 
}

bool ASpaceShip::GetIsDestroyed()
{
	return bIsDestroyed; 
}


void ASpaceShip::StartFire()
{
	if (bIsDestroyed)
	{
		return;
	}
	
	bIsShooting = true;
    
	// Fire the first shot immediately
	SpawnProjectile();
    
	// Start a looping timer that calls SpawnProjectile based on your FireRate
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ASpaceShip::SpawnProjectile, FireRate, true);
}

void ASpaceShip::StopFire()
{
	bIsShooting = false;
    
	// Stop the timer so the ship stops shooting
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ASpaceShip::SpawnProjectile()
{
	FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARocket* SpawnedRocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	// Tell the SpaceShip to ignore the rocket
	if (SpawnedRocket && Hitbox)
	{
		Hitbox->IgnoreActorWhenMoving(SpawnedRocket, true);
	}

	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
}