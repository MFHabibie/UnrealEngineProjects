// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableBlock.h"

// Sets default values
AMovableBlock::AMovableBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");
	TriggerAreaR = CreateDefaultSubobject<UBoxComponent>("TriggerAreaR");
	TriggerAreaL = CreateDefaultSubobject<UBoxComponent>("TriggerAreaL");

	RootComponent = Object;
	TriggerAreaR->SetupAttachment(RootComponent);
	TriggerAreaL->SetupAttachment(RootComponent);

	Object->SetSimulatePhysics(true);
	Object->BodyInstance.bLockXRotation = true;
	Object->BodyInstance.bLockYRotation = true;
	Object->BodyInstance.bLockZRotation = false;

	TriggerAreaR->SetGenerateOverlapEvents(true);
	TriggerAreaR->ComponentTags.Add(FName("BlockPushAreaR"));

	TriggerAreaL->SetGenerateOverlapEvents(true);
	TriggerAreaL->ComponentTags.Add(FName("BlockPushAreaL"));
}

// Called when the game starts or when spawned
void AMovableBlock::BeginPlay()
{
	Super::BeginPlay();
	
	RotationZ = this->GetActorRotation().Yaw;
}

// Called every frame
void AMovableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPushed)
	{
		Object->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		FVector dummyLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * Offset;
		FVector delayLocation = FMath::VInterpTo(this->GetActorLocation(), Player->GetTransform().TransformPosition(FVector(dummyLocation.X, 0.0f, 0.0f)), DeltaTime, 1.0f);

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, Player->GetActorForwardVector().ToString());
		this->SetActorLocation(FVector(dummyLocation.X, dummyLocation.Y, this->GetActorLocation().Z));
	}
	else
	{
		Object->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	this->SetActorRotation(FRotator(this->GetActorRotation().Pitch, RotationZ, this->GetActorRotation().Roll));
}

