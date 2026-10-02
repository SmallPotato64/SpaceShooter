// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Rocket.h"
#include "SpaceShip.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShooterGameModeBase.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	SetRootComponent(Hitbox);
	Hitbox->SetSphereRadius(32.0f);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Hitbox->SetGenerateOverlapEvents(true);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetCollisionProfileName(TEXT("NoCollision"));
	Sprite->SetGenerateOverlapEvents(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;

	// Auto destroy after 20 seconds
	InitialLifeSpan = 20.0f;
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
	float MovementSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * MovementSpeed;
	}
	
	RotationSpeed = FMath::RandRange(-100.0f, 100.0f);
	
	int32 Tier = FMath::RandRange(1, 4);
	Health = Tier;
	
	float ScaleFactor = 1.0f;
	switch (Tier)
	{
		case 1: ScaleFactor = 1.0f; break;
		case 2: ScaleFactor = 1.5f; break;
		case 3: ScaleFactor = 2.0f; break;
		case 4: ScaleFactor = 2.5f; break;
	}
	SetActorScale3D(FVector(ScaleFactor, ScaleFactor, ScaleFactor));
	
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlapBegin);
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Sprite->AddLocalRotation(FRotator(RotationSpeed * DeltaTime, 0.0f, 0.0f));
}

void AAsteroid::ApplyAsteroidDamage(int32 Amount)
{
	Health -= Amount;

	if (HitSound)
	{
		UGameplayStatics::PlaySound2D(this, HitSound);
	}
	
	if (Health <= 0)
	{
		RotationSpeed = 0.0f;
		
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySound2D(this, ExplosionSound);
		}
		
		if (Hitbox)
		{
			Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		if (Sprite && DestroyedSprite)
		{
			Sprite->SetFlipbook(DestroyedSprite);
			Sprite->SetLooping(false);
			Sprite->PlayFromStart();

			SetLifeSpan(0.5f); 
		}
		
		if (ASpaceShooterGameModeBase* GM = Cast<ASpaceShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GM->RecordScoreUpdated();
		}
	}
}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (ARocket* Rocket = Cast<ARocket>(OtherActor))
	{
		ApplyAsteroidDamage(1);
		Rocket->Destroy();
	}
	else if (ASpaceShip* Ship = Cast<ASpaceShip>(OtherActor))
	{
		if (ASpaceShooterGameModeBase* GM = Cast<ASpaceShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (SpaceShipHitSound)
			{
				UGameplayStatics::PlaySound2D(this, SpaceShipHitSound);
			}
			
			GM->RecordLifeUpdated();
		}
		ApplyAsteroidDamage(4);
	}
}