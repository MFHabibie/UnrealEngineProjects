// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UENUM()
enum class EObstacleType : uint8
{
	TRAP,
	GUILLOTINE
};
UCLASS()
class PROJECTALAS_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EObstacleType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* MotionCurve;

	FTimeline TimelineHandle;
	float TimelineValue;
	float StartPosZ;

	UFUNCTION()
		void ObstacleMotion();

	UFUNCTION()
		void ObstacleReset();
};
