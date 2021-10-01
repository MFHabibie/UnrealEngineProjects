// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "Containers/UnrealString.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(39.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);

	//setup properties
	IsOnCurveArea = false;

	//anim montage
	/*GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AMainCharacter::OnNotifyBeginReceived);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(this, &AMainCharacter::OnNotifyEndReceived);*/
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetJumpAllowed(false);
	JumpPower = GetCharacterMovement()->JumpZVelocity;
	PlayerStatus = EStatus::IDLE;
	startingRotationY = -90.0f;
	GetCameraBoom()->SetRelativeRotation(FRotator(0.0f, startingRotationY, 0.0f));
	GetCapsuleComponent()->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f), false);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsOnUpdate) 
	{
		IsOnUpdate = true;
		GetCharacterMovement()->SetJumpAllowed(true);
	}

	this->DeltaTimeFrame = DeltaTime;

	if (PlayerStatus != EStatus::GRIP && PlayerStatus != EStatus::CLIMB)
	{
		if (AxisValue > 0.0f)
		{
			if (!IsOnCurveArea && !IsChangeRotBasedCurve)
			{
				FRotator fixedRotation = FRotator(0.0f, 90.0f, 0.0f);
				this->SetActorRotation(FMath::RInterpTo(GetActorRotation(), fixedRotation, DeltaTimeFrame, 90.0f));
			}
			else
			{
				FRotator fixedRotation = FRotator(0.0f, tempZRotation.Yaw + 90.0f, 0.0f);
				this->SetActorRotation(FMath::RInterpTo(GetActorRotation(), fixedRotation, DeltaTimeFrame, 90.0f));
			}
			IsFacingRight = true;
		}
		else if (AxisValue < 0.0f)
		{
			if (!IsOnCurveArea && !IsChangeRotBasedCurve)
			{
				FRotator fixedRotation = FRotator(0.0f, -90.0f, 0.0f);
				this->SetActorRotation(FMath::RInterpTo(GetActorRotation(), fixedRotation, DeltaTimeFrame, 90.0f));
			}
			else
			{
				FRotator fixedRotation = FRotator(0.0f, tempZRotation.Yaw - 90.0f, 0.0f);
				this->SetActorRotation(FMath::RInterpTo(GetActorRotation(), fixedRotation, DeltaTimeFrame, 90.0f));
			}
			IsFacingRight = false;
		}

		if (GetCharacterMovement()->GetMovementName() == "Falling")
			PlayerStatus = EStatus::JUMP;

		if (AxisValue == 0.0f && GetCharacterMovement()->GetMovementName() != "Falling")
			PlayerStatus = EStatus::IDLE;
	}
	
	if (PlayerStatus == EStatus::PUSHING)
	{
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetRelativeRotation().Yaw, 0.0f));
	}

	GetCameraBoom()->SetRelativeLocation(FVector::ZeroVector);

	if (IsOnCurveArea) 
	{
		FVector newLocation = Path->FindLocationClosestToWorldLocation(GetCapsuleComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);
		GetCapsuleComponent()->SetWorldLocation(FVector(newLocation.X, newLocation.Y, GetCapsuleComponent()->GetComponentLocation().Z), false);

		tempZRotation = Path->FindRotationClosestToWorldLocation(GetCapsuleComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);
	}

	//if(GetCharacterMovement()->GetMovementName())
	//UEnum::GetValueAsString(PlayerStatus);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, UEnum::GetValueAsString(PlayerStatus));

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, GetCapsuleComponent()->GetRelativeRotation().ToString());
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	InputComponent->BindAxis(TEXT("Movement"), this, &AMainCharacter::Moving);

	InputComponent->BindAction("ReleaseGrip", IE_Pressed, this, &AMainCharacter::ReleaseGrip);
	InputComponent->BindAction("ClimbGrip", IE_Pressed, this, &AMainCharacter::StartJump);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::PressInteract);
	InputComponent->BindAction("Interact", IE_Released, this, &AMainCharacter::ReleaseInteract);
	InputComponent->BindAction("Mounting", IE_Pressed, this, &AMainCharacter::Mounting);

	InputComponent->BindTouch(IE_Pressed, this, &AMainCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AMainCharacter::TouchStopped);

}

void AMainCharacter::MoveRight(float Value)
{
	FVector moveDirection;
	if (!IsOnCurveArea && !IsChangeRotBasedCurve)
		moveDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X) * -1.0f;
	else
		moveDirection = Path->FindTangentClosestToWorldLocation(GetCapsuleComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);

	// add movement in that direction
	if (PlayerStatus != EStatus::GRIP && PlayerStatus != EStatus::CLIMB)
	{
		if (PlayerStatus == EStatus::PUSHING) 
		{
			if ((IsNearBlockR && Value < 0.0f) || (IsNearBlockL && Value > 0.0f))
			{
				AddMovementInput(moveDirection, Value);
			}
		}
		else
		{
			AddMovementInput(moveDirection, Value);
		}

		if (Value > 0.0f)
		{
			AxisValue = 1.0f;
		}
		else if (Value < 0.0f)
		{
			AxisValue = -1.0f;
		}
		else
		{
			AxisValue = 0.0f;
		}

		if (Value != 0.0f && PlayerStatus != EStatus::JUMP)
		{
			if (PlayerStatus != EStatus::PUSHING)
			{
				PlayerStatus = EStatus::MOVE;
			}
		}
	}
}

void AMainCharacter::Moving(float Value)
{
	/*if (Value > 0.0f) 
	{
		FRotator newRotation = FRotator(0.0f, -90.0f, 0.0f);
		this->AddActorWorldRotation(newRotation, true);
	}
	else if (Value < 0.0f) 
	{
		FRotator newRotation = FRotator(0.0f, 90.0f, 0.0f);
		this->FaceRotation(newRotation, true);
	}*/

}

void AMainCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AMainCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AMainCharacter::ReleaseGrip()
{
	if (PlayerStatus == EStatus::GRIP)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, ClimbMontage);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		PlayerStatus = EStatus::RELEASE;
	}
}

void AMainCharacter::StartJump()
{
	if (PlayerStatus == EStatus::GRIP)
	{
		FVector NewPosition = FVector(0.0f);
		if (!IsFacingRight)
		{
			NewPosition = FVector(GetActorLocation().X,
								  GetActorLocation().Y + 40.0f,
								  GetActorLocation().Z + 200.0f);
		}
		else
		{
			NewPosition = FVector(GetActorLocation().X,
								  GetActorLocation().Y - 40.0f,
								  GetActorLocation().Z + 200.0f);
		}

		GetMesh()->GetAnimInstance()->Montage_Play(ClimbMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true); 
		//PlayMontage(GetMesh(), ClimbMontage, 1.0f, 0.0f, NAME_None);

		PlayerStatus = EStatus::CLIMB;
		
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		//UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), NewPosition, GetActorRotation(), true, true, DelayGripTime, true, EMoveComponentAction::Move, LatentInfo);
	}
}

void AMainCharacter::Climb()
{
	float DelayGripTime = 0.3f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainCharacter::DelayGrip, DelayGripTime, false);
}

void AMainCharacter::DelayGrip()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	PlayerStatus = EStatus::IDLE;

	GetWorldTimerManager().ClearTimer(UnusedHandle);
}

void AMainCharacter::PressInteract()
{
	if (IsNearBlockR || IsNearBlockL)
	{
		if ((IsNearBlockR && !IsFacingRight) || (IsNearBlockL && IsFacingRight))
		{
			PlayerStatus = EStatus::PUSHING;
			IsPushing = true;
			AMovableBlock* block = Cast<AMovableBlock>(BlockObject);
			block->IsPushed = true;
			block->Player = (ACharacter*)this;

			FVector newLocation = FVector::ZeroVector;
			if(IsFacingRight)
				newLocation = GetMesh()->GetRelativeLocation() - (GetMesh()->GetForwardVector() * 50.0f);
			else
				newLocation = GetMesh()->GetRelativeLocation() + (GetMesh()->GetForwardVector() * 50.0f);

			 GetMesh()->SetRelativeLocation(newLocation);
			 block->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			 GetCharacterMovement()->MaxWalkSpeed = 100.0f;
		}
		else
		{
			PlayerStatus = EStatus::IDLE;
			IsPushing = false;

			FVector newLocation = FVector(0.0f, 0.0f, -97.0f);
			GetMesh()->SetRelativeLocation(newLocation);
			GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetRelativeRotation().Yaw, 0.0f));
			BlockObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
	}
}

void AMainCharacter::ReleaseInteract()
{
	if ((IsNearBlockR && !IsFacingRight) || (IsNearBlockL && IsFacingRight))
	{
		PlayerStatus = EStatus::IDLE;
		IsPushing = false;
		AMovableBlock* block = Cast<AMovableBlock>(BlockObject);
		block->IsPushed = false;

		FVector newLocation = FVector(0.0f, 0.0f, -97.0f);
		GetMesh()->SetRelativeLocation(newLocation);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetRelativeRotation().Yaw, 0.0f));
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AMainCharacter::Mounting()
{
	if (IsOnMountingArea)
	{
		if (Cast<AZChanger>(MountObject)->IsClimbing)
		{
			GetCapsuleComponent()->SetWorldLocation(Cast<AZChanger>(MountObject)->Location, false);
		}
		else
		{
			GetCapsuleComponent()->SetWorldLocation(Cast<AZChanger>(MountObject)->Location, false);
		}
	}
}

void AMainCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACurveArea* CurveArea = nullptr;
	AGripArea* GripArea = nullptr;
	AZChanger* MountArea = nullptr;

	if (OtherComp->ComponentHasTag("GripArea"))
	{
		GripArea = Cast<AGripArea>(OtherActor);
		GripObject = (AActor*)GripArea;

		FVector NewPosition = FVector::ZeroVector;
		if (PlayerStatus == EStatus::JUMP)
		{
			if ((GripArea->IsRightSide && !IsFacingRight) || (!GripArea->IsRightSide && IsFacingRight) || GripArea->IsNotBothSide)
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				GetCharacterMovement()->StopMovementImmediately();

				FVector OffsetPosition = FVector(GripArea->Offset.X, GripArea->Offset.Y, GripArea->Offset.Z);
				FVector GripPosition = GripArea->GetTransform().TransformPosition(OffsetPosition);
				FRotator NewRotation = FRotator(GetCapsuleComponent()->GetRelativeRotation().Pitch, GripArea->GetActorRotation().Yaw, GetCapsuleComponent()->GetRelativeRotation().Roll);

				GetMesh()->GetAnimInstance()->Montage_Play(ClimbMontage, 0.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				GetMesh()->GetAnimInstance()->Montage_Pause(ClimbMontage);

				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), GripPosition, NewRotation, true, true, 0.2f, true, EMoveComponentAction::Move, LatentInfo);
				GetMesh()->SetRelativeRotation(FRotator(0.0f, GetMesh()->GetRelativeRotation().Yaw, 0.0f), false);

				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, GetCameraBoom()->GetRelativeRotation().ToString());

				PlayerStatus = EStatus::GRIP;
			}
		}
	}
	else if (OtherComp->ComponentHasTag("CurveArea"))
	{
		CurveArea = Cast<ACurveArea>(OtherActor);
		CurveObject = (AActor*)CurveArea;

		Path = CurveArea->CurvePath;

		IsOnCurveArea = true;
		IsExitCurveArea = false;

		if (CurveArea->IsRotatingView)
		{
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			FRotator newRotation = FRotator(GetCameraBoom()->GetRelativeRotation().Pitch, CurveArea->rotationY, GetCameraBoom()->GetRelativeRotation().Roll);
			UKismetSystemLibrary::MoveComponentTo(GetCameraBoom(), FVector::ZeroVector, newRotation, true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
			CameraRotationZ = CurveArea->rotationY;
		}
	}
	else if (OtherComp->ComponentHasTag("EnterCurve"))
	{
		CurveArea = Cast<ACurveArea>(OtherActor);
		if (!CurveArea->IsEnterNotSet && CurveArea->sideTurn)
		{
			CurveArea->IsEnterNotSet = true;
			CurveArea->EnterRotationY = GetCameraBoom()->GetRelativeRotation().Yaw;
		}

		if (CurveArea->sideTurn)
			IsExitCurveArea = true;

		if (CurveArea->IsRotatingView)
		{
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;

			FRotator newRotation = FRotator(GetCameraBoom()->GetRelativeRotation().Pitch, CurveArea->EnterRotationY, GetCameraBoom()->GetRelativeRotation().Roll);
			UKismetSystemLibrary::MoveComponentTo(GetCameraBoom(), FVector::ZeroVector, newRotation, true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
			CameraRotationZ = CurveArea->EnterRotationY;
		}
	}
	else if (OtherComp->ComponentHasTag("ExitCurve"))
	{
		CurveArea = Cast<ACurveArea>(OtherActor);

		if (CurveArea->sideTurn)
		{
			IsChangeRotBasedCurve = true;
			IsExitCurveArea = true;
		}
	}
	else if (OtherComp->ComponentHasTag("Coin"))
	{
		ACoin* coin = Cast<ACoin>(OtherActor);

		CoinStored++;
		coin->GetCoin();
	}
	else if (OtherComp->ComponentHasTag("BlockPushAreaR"))
	{
		BlockObject = OtherActor;
		IsNearBlockR = true;
	}
	else if (OtherComp->ComponentHasTag("BlockPushAreaL"))
	{
		BlockObject = OtherActor;
		IsNearBlockL = true;
	}
	else if (OtherComp->ComponentHasTag("MountArea"))
	{
		MountArea = Cast<AZChanger>(OtherActor);
		MountObject = (AActor*)MountArea;

		IsOnMountingArea = true;
	}
}

void AMainCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACurveArea* CurveArea = nullptr;

	if (OtherComp->ComponentHasTag("CurveArea"))
	{
		CurveArea = Cast<ACurveArea>(OtherActor);

		if (IsExitCurveArea || !CurveArea->sideTurn)
		{
			IsOnCurveArea = false;

			if (CurveArea->IsRotatingView && !CurveArea->sideTurn)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Detect")));
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				FRotator newRotation = FRotator(GetCameraBoom()->GetRelativeRotation().Pitch, CurveArea->EnterRotationY, GetCameraBoom()->GetRelativeRotation().Roll);
				UKismetSystemLibrary::MoveComponentTo(GetCameraBoom(), FVector::ZeroVector, newRotation, true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
				CameraRotationZ = startingRotationY;
			}
		}
	}
	else if (OtherComp->ComponentHasTag("ExitCurve") && !IsOnCurveArea)
	{
		CurveArea = Cast<ACurveArea>(OtherActor);
		FLatentActionInfo LatentInfo;

		if (CurveArea->sideTurn)
		{
			IsChangeRotBasedCurve = true;
			IsExitCurveArea = true;
			LatentInfo.CallbackTarget = this;
		}

		if (CurveArea->IsRotatingView)
		{
			FRotator newRotation = FRotator(GetCameraBoom()->GetRelativeRotation().Pitch, CurveArea->ExitRotationY, GetCameraBoom()->GetRelativeRotation().Roll);
			UKismetSystemLibrary::MoveComponentTo(GetCameraBoom(), FVector::ZeroVector, newRotation, true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
			CameraRotationZ = CurveArea->ExitRotationY;
		}
	}
	else if (OtherComp->ComponentHasTag("BlockPushAreaR"))
	{
		AMovableBlock* block = Cast<AMovableBlock>(BlockObject);
		block->IsPushed = false;

		IsNearBlockR = false;

		PlayerStatus = EStatus::IDLE;
		IsPushing = false;

		FVector newLocation = FVector(0.0f, 0.0f, -97.0f);
		GetMesh()->SetRelativeLocation(newLocation);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetRelativeRotation().Yaw, 0.0f));
		BlockObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (OtherComp->ComponentHasTag("BlockPushAreaL"))
	{
		AMovableBlock* block = Cast<AMovableBlock>(BlockObject);
		block->IsPushed = false;

		IsNearBlockL = false;

		PlayerStatus = EStatus::IDLE;
		IsPushing = false;

		FVector newLocation = FVector(0.0f, 0.0f, -97.0f);
		GetMesh()->SetRelativeLocation(newLocation);
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, GetCapsuleComponent()->GetRelativeRotation().Yaw, 0.0f));
		BlockObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (OtherComp->ComponentHasTag("MountArea"))
	{
		MountObject = nullptr;

		IsOnMountingArea = false;
	}
}

