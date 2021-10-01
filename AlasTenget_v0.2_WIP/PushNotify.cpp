// Fill out your copyright notice in the Description page of Project Settings.


#include "PushNotify.h"
#include "MovableBlock.h"
#include "Engine.h"
#include "EngineGlobals.h"

void UPushNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Read")));
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, Player->GetFName().ToString());

	/*AMainCharacter* Player = Cast<AMainCharacter>(MeshComp->GetAttachParent()->GetAttachmentRootActor());

	FVector OffsetPosition = FVector(50.0f, 0.0f, 30.0f);
	FVector PushPosition = Player->BlockObject->GetTransform().TransformPosition(OffsetPosition);
	FRotator NewRotation = FRotator(-45.0f, Player->GetCapsuleComponent()->GetRelativeRotation().Yaw, Player->GetCapsuleComponent()->GetRelativeRotation().Roll);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(), PushPosition, NewRotation, true, true, 0.2f, true, EMoveComponentAction::Move, LatentInfo);*/
}