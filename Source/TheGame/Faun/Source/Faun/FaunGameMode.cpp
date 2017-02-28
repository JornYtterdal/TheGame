// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Faun.h"
#include "FaunGameMode.h"
#include "FaunPlayerController.h"
#include "FaunCharacter.h"

AFaunGameMode::AFaunGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFaunPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}