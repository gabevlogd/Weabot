// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Generic/SaveSystem/Data/Structs/SaveSlotData.h"
#include "DefaultSaveGame.generated.h"


UCLASS()
class PROJECTAI_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	FSaveSlotData SaveSlotData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame()
	{
		SaveSlotData = FSaveSlotData();
		ActorTransforms = TMap<FName, FTransform>();
	}

	void CreateSlotData(const FString& SlotName)
	{
		SaveSlotData = FSaveSlotData();
		SaveSlotData.SlotName = SlotName;
	}

	bool operator<(const UDefaultSaveGame& Other) const
	{
		return SaveSlotData.LastSaveDate < Other.SaveSlotData.LastSaveDate;
	}

	bool operator>(const UDefaultSaveGame& Other) const
	{
		return SaveSlotData.LastSaveDate > Other.SaveSlotData.LastSaveDate;
	}
};
