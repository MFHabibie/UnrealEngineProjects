// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "AreaJumpThrough.generated.h"

UENUM()
enum class EType : uint8
{
	DifZ,
	SameZ
};

UCLASS()
class PROJECTALAS_API AAreaJumpThrough : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaJumpThrough();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere)
		UBoxComponent* UpperArea;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BelowArea;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBelowPlatform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		EType PlatformType;
};
