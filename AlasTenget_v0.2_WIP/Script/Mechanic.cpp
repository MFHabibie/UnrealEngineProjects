// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanic.h"

// Sets default values
AMechanic::AMechanic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");

	Object->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMechanic::BeginPlay()
{
	Super::BeginPlay();
	
	GetComponents<UBoxComponent>(MechanicItems);
	TotalObjectsIn.SetNum(MechanicItems.Num(), true);
	TriggerActivate = false;
	
}

// Called every frame
void AMechanic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TriggerActivate && CheckTriggered())
	{
		TriggerActivate = true;
		AGate* gateObject = Cast<AGate>(GateObject);
		gateObject->OpenGate();
	}
	else if(!CheckTriggered())
	{
		TriggerActivate = false;
		AGate* gateObject = Cast<AGate>(GateObject);
		gateObject->CloseGate();
	}
}

void AMechanic::PressTrigger(UStaticMeshComponent* objectTrigger, UBoxComponent* objectArea)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = objectTrigger;

	int indexItem = 0;
	for (int i = 0; i < MechanicItems.Num(); i++)
	{
		if (MechanicItems[i] == objectArea)
		{
			indexItem = i;
		}
	}

	TotalObjectsIn[indexItem]++;

	if (objectTrigger->GetRelativeLocation().Z != -70.0f && TotalObjectsIn[indexItem] == 1)
	{
		FVector newLocation = FVector(objectTrigger->GetRelativeLocation().X, objectTrigger->GetRelativeLocation().Y, -70.0f);
		UKismetSystemLibrary::MoveComponentTo(objectTrigger, newLocation, objectTrigger->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
		
		TriggerFinish++;
	}
}

void AMechanic::ReleaseTrigger(UStaticMeshComponent* objectTrigger, UBoxComponent* objectArea)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = objectTrigger;

	int indexItem = 0;
	for (int i = 0; i < MechanicItems.Num(); i++)
	{
		if (MechanicItems[i] == objectArea)
		{
			indexItem = i;
		}
	}

	TotalObjectsIn[indexItem]--;

	if (TotalObjectsIn[indexItem] < 1)
	{
		FVector newLocation = FVector(objectTrigger->GetRelativeLocation().X, objectTrigger->GetRelativeLocation().Y, -30.0f);
		UKismetSystemLibrary::MoveComponentTo(objectTrigger, newLocation, objectTrigger->GetRelativeRotation(), true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);
		
		TriggerFinish--;
	}
}

bool AMechanic::CheckTriggered()
{
	if (TriggerFinish == TotalTrigger)
		return true;
	else
		return false;
}

