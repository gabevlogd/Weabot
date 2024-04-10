// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LastSelectedSlotSave.generated.h"


UCLASS()
class PROJECTAI_API ULastSelectedSlotSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LastSelectedSaveSlotName;
};
