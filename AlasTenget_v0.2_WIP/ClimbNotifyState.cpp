// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbNotifyState.h"
#include "Engine.h"

void UClimbNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//AMainCharacter* Player = Cast<AMainCharacter>(MeshComp->GetAttachParent()->GetAttachmentRoot());
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, Player->GetFName().ToString());
}

void UClimbNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Tick")));
}

void UClimbNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("End")));
}
