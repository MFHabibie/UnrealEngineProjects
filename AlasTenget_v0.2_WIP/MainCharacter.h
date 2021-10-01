// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GripArea.h"
#include "CurveArea.h"
#include "ZChanger.h"
#include "MovableBlock.h"
#include "Coin.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM()
enum class EStatus : uint8
{
	IDLE,
	MOVE,
	CRAWL,
	JUMP,
	GRIP,
	CLIMB,
	RELEASE,
	PUSHING
};

UCLASS()
class PROJECTALAS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	UFUNCTION()
	/** Called for side to side input */
	void Moving(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	void ReleaseGrip();

	void StartJump();

	void DelayGrip();

	void PressInteract();

	void ReleaseInteract();

	void Mounting();

	float startingRotationY;

	FTimerHandle UnusedHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		EStatus PlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
		UAnimMontage* ClimbMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
		UAnimMontage* PushMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		float JumpPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		float AxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		float DeltaTimeFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		float CameraRotationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		int CoinStored;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		bool IsOnUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		bool IsFacingRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetupPlayer")
		bool IsPushing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsOnCurveArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsExitCurveArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsChangeRotBasedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsNearBlockR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsNearBlockL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerAction")
		bool IsOnMountingArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectReference")
		USplineComponent* Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectReference")
		AActor* GripObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectReference")
		AActor* CurveObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectReference")
		AActor* BlockObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectReference")
		AActor* MountObject;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Climb();

	FRotator tempZRotation;

	//playingmontage
	/*UFUNCTION()
		void PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent, class UAnimMontage* MontageToPlay, float PlayRate, float StartingPosition, FName StartingSection);*/

	/*UFUNCTION()
		void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/

	/*UFUNCTION()
		void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
		void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);*/
};
