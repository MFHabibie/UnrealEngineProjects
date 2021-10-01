// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAlasGameMode.h"
#include "ProjectAlasCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectAlasGameMode::AProjectAlasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
