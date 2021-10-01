// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaJumpThrough.h"
#include "MainCharacter.h"

// Sets default values
AAreaJumpThrough::AAreaJumpThrough()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>("Object");
	BelowArea = CreateDefaultSubobject<UBoxComponent>("BelowArea");
	UpperArea = CreateDefaultSubobject<UBoxComponent>("UpperArea");

	RootComponent = Object;

	BelowArea->SetupAttachment(RootComponent);
	UpperArea->SetupAttachment(RootComponent);

	Object->SetCollisionProfileName(TEXT("JumpThrough"));
	Object->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Object->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	BelowArea->SetGenerateOverlapEvents(true);
	UpperArea->SetGenerateOverlapEvents(true);

	BelowArea->OnComponentBeginOverlap.AddDynamic(this, &AAreaJumpThrough::OnOverlapBegin);
	BelowArea->OnComponentEndOverlap.AddDynamic(this, &AAreaJumpThrough::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AAreaJumpThrough::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaJumpThrough::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAreaJumpThrough::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* player = Cast<AMainCharacter>(OtherActor);

	if (OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Overlap")));
		if (PlatformType == EType::SameZ)
		{
			Object->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
		else if (PlatformType == EType::DifZ)
		{
			if (IsBelowPlatform)
			{
				//player->
			}
		}
	}
}

void AAreaJumpThrough::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (PlatformType == EType::SameZ)
		{
			//Object->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else if (PlatformType == EType::DifZ)
		{
			if (IsBelowPlatform)
			{
				//player->
			}
		}
	}
}

