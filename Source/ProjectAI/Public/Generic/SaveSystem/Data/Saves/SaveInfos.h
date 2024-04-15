// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Generic/SaveSystem/Data/Structs/SlotInfoData.h"
#include "SaveInfos.generated.h"


UCLASS()
class PROJECTAI_API USaveInfos : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FSlotInfoData> SlotInfos;

	USaveInfos()
	{
		SlotInfos = TMap<FName, FSlotInfoData>();
	}
};
