// Fill out your copyright notice in the Description page of Project Settings.


#include "ZChanger.h"

// Sets default values
AZChanger::AZChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");
	AreaTrigger = CreateDefaultSubobject<UBoxComponent>("AreaTrigger");

	RootComponent = Object;

	AreaTrigger->SetupAttachment(RootComponent);

	AreaTrigger->SetGenerateOverlapEvents(true);
	AreaTrigger->ComponentTags.Add(FName("MountArea"));

	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &AZChanger::OnOverlapBegin);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &AZChanger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AZChanger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZChanger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AZChanger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

