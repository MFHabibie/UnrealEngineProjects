// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PushNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALAS_API UPushNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FTimerHandle UnusedHandle;
};
