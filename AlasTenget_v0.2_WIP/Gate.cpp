// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Gate = CreateDefaultSubobject<UStaticMeshComponent>("Gate");
	LeftGate = CreateDefaultSubobject<UStaticMeshComponent>("LeftGate");
	RightGate = CreateDefaultSubobject<UStaticMeshComponent>("RightGate");
	TriggerArea = CreateDefaultSubobject<UBoxComponent>("TriggerArea");

	RootComponent = Gate;

	LeftGate->SetupAttachment(RootComponent);
	RightGate->SetupAttachment(RootComponent);
	TriggerArea->SetupAttachment(RootComponent);

	TriggerArea->SetGenerateOverlapEvents(true);

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBegin);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &AGate::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();
	
	StartRotationL = LeftGate->GetRelativeRotation().Yaw;
	StartRotationR = RightGate->GetRelativeRotation().Yaw;
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGate::OpenGate()
{
	FLatentActionInfo LatentInfoR;
	FLatentActionInfo LatentInfoL;
	LatentInfoR.CallbackTarget = RightGate;
	LatentInfoL.CallbackTarget = LeftGate;

	if (LeftGate->GetRelativeRotation().Yaw != 90.0f)
	{
		FRotator newRotationL = FRotator(LeftGate->GetRelativeRotation().Pitch, 90.0f, LeftGate->GetRelativeRotation().Roll);
		FRotator newRotationR = FRotator(RightGate->GetRelativeRotation().Pitch, -90.0f, RightGate->GetRelativeRotation().Roll);
		UKismetSystemLibrary::MoveComponentTo(LeftGate, LeftGate->GetRelativeLocation(), newRotationL, false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoL);
		UKismetSystemLibrary::MoveComponentTo(RightGate, RightGate->GetRelativeLocation(), newRotationR, false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoR);
	}
}

void AGate::CloseGate()
{
	FLatentActionInfo LatentInfoR;
	FLatentActionInfo LatentInfoL;
	LatentInfoR.CallbackTarget = RightGate;
	LatentInfoL.CallbackTarget = LeftGate;

	if (LeftGate->GetRelativeRotation().Yaw != 0.0f || RightGate->GetRelativeRotation().Yaw != 0.0f)
	{
		FRotator newRotationL = FRotator(LeftGate->GetRelativeRotation().Pitch, StartRotationL, LeftGate->GetRelativeRotation().Roll);
		FRotator newRotationR = FRotator(RightGate->GetRelativeRotation().Pitch, StartRotationR, RightGate->GetRelativeRotation().Roll);
		UKismetSystemLibrary::MoveComponentTo(LeftGate, LeftGate->GetRelativeLocation(), newRotationL, false, false, 0.5f, true, EMoveComponentAction::Move, LatentInfoL);
		UKismetSystemLibrary::MoveComponentTo(RightGate, RightGate->GetRelativeLocation(), newRotationR, false, false, 0.5f, true, EMoveComponentAction::Move, LatentInfoR);
	}
}

void AGate::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMainCharacter>(OtherActor))
	{
		AMainCharacter* chara = Cast<AMainCharacter>(OtherActor);

		if (GateType == EGateType::COIN)
		{
			if (chara->CoinStored >= CoinNeeded && !GateClosed)
			{
				GateClosed = true;
				chara->CoinStored = chara->CoinStored - CoinNeeded;
				OpenGate();
			}
		}
	}
}

void AGate::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

	}
}
