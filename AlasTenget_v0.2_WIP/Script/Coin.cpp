// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");

	Object->SetupAttachment(RootComponent);

	Object->SetGenerateOverlapEvents(true);
	Object->ComponentTags.Add(FName("Coin"));
	Object->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Object->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Object->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Object->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);
	Object->OnComponentEndOverlap.AddDynamic(this, &ACoin::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();

	Object->SetRelativeRotation(FRotator(Object->GetRelativeRotation().Pitch, 0.0f, Object->GetRelativeRotation().Roll));

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishCallback;

	TimelineCallback.BindUFunction(this, FName("CoinRotator"));
	TimelineFinishCallback.BindUFunction(this, FName("CoinReset"));

	TimerHandle.AddInterpFloat(CoinCurve, TimelineCallback);
	TimerHandle.SetTimelineFinishedFunc(TimelineFinishCallback);
	TimerHandle.SetLooping(true);
	TimerHandle.PlayFromStart();
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimerHandle.TickTimeline(DeltaTime);
}

void ACoin::CoinRotator()
{
	TimelineValue = CoinCurve->GetFloatValue(TimerHandle.GetPlaybackPosition());

	float RotateValue = TimelineValue * 360.0f;
	Object->SetRelativeRotation(FRotator(Object->GetRelativeRotation().Pitch, RotateValue, Object->GetRelativeRotation().Roll));
}

void ACoin::CoinReset()
{
	Object->SetRelativeRotation(FRotator(Object->GetRelativeRotation().Pitch, 0.0f, Object->GetRelativeRotation().Roll));
}

void ACoin::GetCoin()
{
	TimerHandle.Stop();
	Destroy();
}

void ACoin::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{

	}
}

void ACoin::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

	}
}

