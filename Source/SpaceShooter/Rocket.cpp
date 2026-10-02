// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(Hitbox);
	Hitbox->SetSphereRadius(32.0f);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Hitbox->SetGenerateOverlapEvents(true);
	
	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipFlipbook"));
	Sprite->SetupAttachment(RootComponent);
	
	Sprite->SetCollisionProfileName(TEXT("NoCollision"));
	Sprite->SetGenerateOverlapEvents(false);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bShouldBounce = false;
	
	InitialLifeSpan = 20.0f;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		Hitbox->IgnoreActorWhenMoving(GetOwner(), true);
	}
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

