// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"
#include "TriggerMachine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALAS_API UTriggerMachine : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerMachine();

protected:
	

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerArea;

	UFUNCTION()
		void PressTrigger();

	UFUNCTION()
		void ReleaseTrigger();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsPressed;
};
