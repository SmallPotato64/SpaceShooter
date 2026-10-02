// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceShooterGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdatedSignature, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLifeUpdatedSignature, int32, Life);


UCLASS()
class SPACESHOOTER_API ASpaceShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASpaceShooterGameModeBase();
	
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreUpdatedSignature OnScoreUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Life")
	FOnLifeUpdatedSignature OnLifeUpdated;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> GameOverSound;
	
	void RecordScoreUpdated();
	void RecordLifeUpdated();
	
	UFUNCTION(BlueprintPure, Category = "Scoring")
	int32 GetScore();

	int32 Score;
	int32 Life;
};
