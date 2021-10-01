// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbNotify.h"
#include "GripArea.h"
#include "Engine.h"
#include "EngineGlobals.h"

void UClimbNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Read")));
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, Player->GetFName().ToString());

	AMainCharacter* Player = Cast<AMainCharacter>(MeshComp->GetAttachParent()->GetAttachmentRootActor());

	FVector OffsetPosition = Cast<AGripArea>(Player->GripObject)->OffsetClimb;
	FVector GripPosition = Player->GripObject->GetTransform().TransformPosition(OffsetPosition);
	FRotator NewRotation = FRotator(0.0f, Player->GetActorRotation().Yaw, 0.0f);

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, NewPosition.ToString());

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	float DelayGripTime = 0.3f;
	UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(), GripPosition, NewRotation, true, true, DelayGripTime, true, EMoveComponentAction::Move, LatentInfo);

	Player->Climb();
}