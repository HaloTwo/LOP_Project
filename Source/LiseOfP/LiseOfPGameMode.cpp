// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiseOfPGameMode.h"
#include "LiseOfPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALiseOfPGameMode::ALiseOfPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
