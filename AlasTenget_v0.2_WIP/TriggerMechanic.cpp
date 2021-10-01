// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerMechanic.h"

UTriggerMechanic::UTriggerMechanic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");

	Trigger->SetGenerateOverlapEvents(true);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &UTriggerMechanic::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &UTriggerMechanic::OnOverlapEnd);
}

void UTriggerMechanic::PressTrigger()
{
	Object = (UStaticMeshComponent*)this->GetChildComponent(0);
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = Object;

	FVector newLocation = FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, -30.0f);
	UKismetSystemLibrary::MoveComponentTo(Object, newLocation, Object->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
}

void UTriggerMechanic::ReleaseTrigger()
{
	Object = (UStaticMeshComponent*)this->GetChildComponent(0);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = Object;

	FVector newLocation = FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, 0.0f);
	UKismetSystemLibrary::MoveComponentTo(Object, newLocation, Object->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
}

void UTriggerMechanic::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, OtherActor->GetName());
	PressTrigger();
}

void UTriggerMechanic::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ReleaseTrigger();
}