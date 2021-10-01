// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TriggerMechanic.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALAS_API UTriggerMechanic : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerMechanic(const FObjectInitializer& ObjectInitializer);

public:

	FComponentBeginOverlapSignature StartOverlappingDelegates;
	FComponentEndOverlapSignature EndOverlappingDelegates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY()
		UBoxComponent* Trigger;

	UFUNCTION(BlueprintCallable)
		void PressTrigger();

	UFUNCTION()
		void ReleaseTrigger();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPressed;
};
