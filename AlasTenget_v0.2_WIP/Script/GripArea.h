// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GripArea.generated.h"

UCLASS()
class PROJECTALAS_API AGripArea : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGripArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* Area;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRightSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsNotBothSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector OffsetClimb;
};
