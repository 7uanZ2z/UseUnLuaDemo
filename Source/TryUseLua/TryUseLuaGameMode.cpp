// Copyright Epic Games, Inc. All Rights Reserved.

#include "TryUseLuaGameMode.h"
#include "TryUseLuaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATryUseLuaGameMode::ATryUseLuaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
