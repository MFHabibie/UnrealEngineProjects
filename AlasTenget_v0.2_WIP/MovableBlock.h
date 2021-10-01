// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "MovableBlock.generated.h"

UCLASS()
class PROJECTALAS_API AMovableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerAreaR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerAreaL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPushed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Offset;
};
