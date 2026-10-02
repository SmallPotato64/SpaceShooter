// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "SpaceShip.h"
#include "SpaceShipAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API USpaceShipAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<ASpaceShip> OwningSpaceShip;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsShooting;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsDestroyed;
	
	virtual void OnInit_Implementation() override;
	virtual void OnTick_Implementation(float DeltaTime) override;
};
