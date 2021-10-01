// Fill out your copyright notice in the Description page of Project Settings.


#include "GripArea.h"

// Sets default values
AGripArea::AGripArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Area = CreateDefaultSubobject<UBoxComponent>("Area");

	RootComponent = Area;

	Area->SetGenerateOverlapEvents(true);
	Area->ComponentTags.Add(FName("GripArea"));

	Area->OnComponentBeginOverlap.AddDynamic(this, &AGripArea::OnOverlapBegin);
	Area->OnComponentEndOverlap.AddDynamic(this, &AGripArea::OnOverlapEnd);

	OffsetClimb = FVector(100.0f, 0.0f, 230.0f);
}

// Called when the game starts or when spawned
void AGripArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGripArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGripArea::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Overlap")));
	if (OtherComp->GetFName() == "TouchArea")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Overlap")));

	}
}

void AGripArea::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherComp->GetFName() == "TouchArea")
	{

	}
}

