// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Gate.generated.h"

UENUM()
enum class EGateType : uint8
{
	MECH,
	COIN
};
UCLASS()
class PROJECTALAS_API AGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGateType GateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Gate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LeftGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RightGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GateClosed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CoinNeeded;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OpenGate();

	UFUNCTION()
		void CloseGate();

	float StartRotationL;
	float StartRotationR;
};
