// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveArea.h"

// Sets default values
ACurveArea::ACurveArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");
	CurvePath = CreateDefaultSubobject<USplineComponent>("CurvePath");
	AreaTrigger = CreateDefaultSubobject<UBoxComponent>("AreaTrigger");
	AreaTriggerEnter = CreateDefaultSubobject<UBoxComponent>("EnterRotationY");
	AreaTriggerExit = CreateDefaultSubobject<UBoxComponent>("ExitRotationY");

	RootComponent = Object;

	CurvePath->SetupAttachment(RootComponent);
	AreaTrigger->SetupAttachment(RootComponent);
	AreaTriggerEnter->SetupAttachment(RootComponent);
	AreaTriggerExit->SetupAttachment(RootComponent);

	AreaTrigger->SetGenerateOverlapEvents(true);
	AreaTrigger->ComponentTags.Add(FName("CurveArea"));
	AreaTriggerEnter->ComponentTags.Add(FName("EnterCurve"));
	AreaTriggerExit->ComponentTags.Add(FName("ExitCurve"));

	AreaTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACurveArea::OnOverlapBegin);
	AreaTrigger->OnComponentEndOverlap.AddDynamic(this, &ACurveArea::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ACurveArea::BeginPlay()
{
	Super::BeginPlay();
	
	if (sideTurn) 
	{
		AreaTriggerEnter->SetGenerateOverlapEvents(true);
		AreaTriggerExit->SetGenerateOverlapEvents(true);
	}
	else
	{
		AreaTriggerEnter->SetGenerateOverlapEvents(false);
		AreaTriggerExit->SetGenerateOverlapEvents(false);
	}
}

// Called every frame
void ACurveArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (IsOverlapp)
	{
		FVector newLocation = CurvePath->FindLocationClosestToWorldLocation(BlockObject->GetActorLocation(), ESplineCoordinateSpace::World);
		BlockObject->SetActorLocation(FVector(newLocation.X, newLocation.Y, BlockObject->GetActorLocation().Z));
	}*/

}

void ACurveArea::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMovableBlock>(OtherActor))
	{
		BlockObject = OtherActor;
		AMovableBlock* block = Cast<AMovableBlock>(OtherActor);

		FVector newLocation = CurvePath->FindLocationClosestToWorldLocation(OtherActor->GetActorLocation(), ESplineCoordinateSpace::World);
		FRotator newRotation = CurvePath->FindRotationClosestToWorldLocation(OtherActor->GetActorLocation(), ESplineCoordinateSpace::World);

		block->RotationZ = newRotation.Yaw;
		block->SetActorLocation(FVector(newLocation.X, newLocation.Y, OtherActor->GetActorLocation().Z));
		block->SetActorRotation(newRotation);

		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACurveArea::ResetBlock, 0.2f, false);
	}

}

void ACurveArea::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ACurveArea::ResetBlock()
{
	AMovableBlock* block = Cast<AMovableBlock>(BlockObject);
	block->SetActorRotation(FRotator(0.0f, block->RotationZ, 0.0f));
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::SanitizeFloat(block->RotationZ));
	GetWorldTimerManager().ClearTimer(UnusedHandle);
}