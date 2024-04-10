// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlotsInfoSave.generated.h"


UCLASS()
class PROJECTAI_API USlotsInfoSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LastSelectedSaveSlotName;
};
