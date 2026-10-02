// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipAnimInstance.h"
#include "SpaceShip.h"

void USpaceShipAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();

	OwningSpaceShip = Cast<ASpaceShip>(GetOwningActor());
}

void USpaceShipAnimInstance::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);

	if (!OwningSpaceShip)
	{
		OwningSpaceShip = Cast<ASpaceShip>(GetOwningActor());
	}
	
	if (OwningSpaceShip)
	{
		bIsMoving = OwningSpaceShip->GetIsMoving();
		bIsShooting = OwningSpaceShip->GetIsShooting();
		bIsDestroyed = OwningSpaceShip->GetIsDestroyed();
	}
}

