// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");

	Object->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	if (Type == EObstacleType::TRAP)
		StartPosZ = Object->GetRelativeLocation().Z;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishCallback;

	TimelineCallback.BindUFunction(this, FName("ObstacleMotion"));
	TimelineFinishCallback.BindUFunction(this, FName("ObstacleReset"));

	TimelineHandle.SetTimelineFinishedFunc(TimelineFinishCallback);
	TimelineHandle.AddInterpFloat(MotionCurve, TimelineCallback);
	TimelineHandle.SetLooping(true);
	TimelineHandle.PlayFromStart();
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimelineHandle.TickTimeline(DeltaTime);
}

void AObstacle::ObstacleMotion()
{
	TimelineValue = MotionCurve->GetFloatValue(TimelineHandle.GetPlaybackPosition());

	if (Type == EObstacleType::TRAP)
	{
		float value = TimelineValue * -300.0f;

		Object->SetRelativeLocation(FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, StartPosZ + value));
	}
	else if (Type == EObstacleType::GUILLOTINE)
	{
		float value = TimelineValue * 60.0f;

		Object->SetRelativeRotation(FRotator(value, Object->GetRelativeRotation().Yaw, Object->GetRelativeRotation().Roll));
	}
}

void AObstacle::ObstacleReset()
{
	if (Type == EObstacleType::TRAP)
	{
		Object->SetRelativeLocation(FVector(Object->GetRelativeLocation().X, Object->GetRelativeLocation().Y, 0.0f));
	}
	else if (Type == EObstacleType::GUILLOTINE)
	{
		Object->SetRelativeRotation(FRotator(Object->GetRelativeRotation().Pitch, Object->GetRelativeRotation().Yaw, Object->GetRelativeRotation().Roll));
	}
}