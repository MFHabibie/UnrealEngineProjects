// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerMechanic.h"
#include "Gate.h"
#include "GameFramework/Actor.h"
#include "Mechanic.generated.h"

UCLASS()
class PROJECTALAS_API AMechanic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMechanic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UBoxComponent*> MechanicItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> TotalObjectsIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* GateObject;

	UFUNCTION(BlueprintCallable)
		void PressTrigger(UStaticMeshComponent* objectTrigger, UBoxComponent* objectArea);

	UFUNCTION(BlueprintCallable)
		void ReleaseTrigger(UStaticMeshComponent* objectTrigger, UBoxComponent* objectArea);

	UFUNCTION()
		bool CheckTriggered();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TotalTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TriggerFinish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TriggerActivate;

};
