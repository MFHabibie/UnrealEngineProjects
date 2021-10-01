// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerMachine.h"

// Sets default values for this component's properties
UTriggerMachine::UTriggerMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
	TriggerArea = CreateDefaultSubobject<UBoxComponent>("TriggerArea");

	TriggerArea->SetGenerateOverlapEvents(true);

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &UTriggerMachine::OnOverlapBegin);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &UTriggerMachine::OnOverlapEnd);
}

void UTriggerMachine::PressTrigger()
{
	Object = (UStaticMeshComponent*)this->GetChildComponent(0);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = Object;

	FVector newLocation = FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, -30.0f);
	UKismetSystemLibrary::MoveComponentTo(Object, newLocation, Object->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
}

void UTriggerMachine::ReleaseTrigger()
{
	Object = (UStaticMeshComponent*)this->GetChildComponent(0);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = Object;

	FVector newLocation = FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, 0.0f);
	UKismetSystemLibrary::MoveComponentTo(Object, newLocation, Object->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
}

void UTriggerMachine::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PressTrigger();
}

void UTriggerMachine::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ReleaseTrigger();
}