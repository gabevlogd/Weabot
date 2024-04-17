// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Generic/SaveSystem/Data/Structs/SlotInfoData.h"
#include "SlotInfos.generated.h"


UCLASS()
class PROJECTAI_API USlotInfos : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FSlotInfoData> SlotInfos;

	USlotInfos()
	{
		SlotInfos = TMap<FName, FSlotInfoData>();
	}
};
