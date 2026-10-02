// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShooterGameModeBase.h"
#include "SpaceShip.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ASpaceShooterGameModeBase::ASpaceShooterGameModeBase()
{
	Score = 0;
	Life = 3;
}

void ASpaceShooterGameModeBase::RecordScoreUpdated()
{
	Score += 100;
	OnScoreUpdated.Broadcast(Score);
}

void ASpaceShooterGameModeBase::RecordLifeUpdated()
{
	Life--;

	OnLifeUpdated.Broadcast(Life);
	
	if (Life == 0)
	{
		if (GameOverSound)
		{
			UGameplayStatics::PlaySound2D(this, GameOverSound);
		}
		
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		
		if (ASpaceShip* MyShip = Cast<ASpaceShip>(PlayerPawn))
		{
			MyShip->Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyShip->MovementComponent->MaxSpeed = 0;
			MyShip->bIsDestroyed = true;
			MyShip->SetLifeSpan(0.6f);
		}
	}
}

int32 ASpaceShooterGameModeBase::GetScore()
{
	return Score;
}