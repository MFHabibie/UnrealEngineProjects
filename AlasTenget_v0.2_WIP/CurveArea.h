// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "MovableBlock.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CurveArea.generated.h"

UCLASS()
class PROJECTALAS_API ACurveArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurveArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* CurvePath;

	UPROPERTY(EditAnywhere)
		UBoxComponent* AreaTrigger;

	UPROPERTY(EditAnywhere)
		UBoxComponent* AreaTriggerEnter;

	UPROPERTY(EditAnywhere)
		UBoxComponent* AreaTriggerExit;

	UPROPERTY(EditAnywhere)
		float rotationY;

	UPROPERTY(EditAnywhere)
		float EnterRotationY;

	UPROPERTY(EditAnywhere)
		float ExitRotationY;

	UPROPERTY(EditAnywhere)
		bool IsRotatingView;

	UPROPERTY(EditAnywhere)
		bool IsEnterNotSet;

	UPROPERTY(EditAnywhere)
		bool sideTurn;

	UPROPERTY(EditAnywhere)
		bool IsResetter;

	UPROPERTY(EditAnywhere)
		bool IsOverlapp;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	FTimerHandle UnusedHandle;
	AActor* BlockObject;

	UFUNCTION()
		void ResetBlock();
};
