// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "..\Structs\SlotInfoData.h"
#include "DefaultSaveGame.generated.h"


UCLASS()
class PROJECTAI_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	FSlotInfoData SlotInfoData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame()
	{
		SlotInfoData = FSlotInfoData();
		ActorTransforms = TMap<FName, FTransform>();
	}

	void CreateSlotInfoData(const FString& SlotName)
	{
		SlotInfoData = FSlotInfoData();
		SlotInfoData.SlotName = SlotName;
	}

	bool operator<(const UDefaultSaveGame& Other) const
	{
		return SlotInfoData.LastSaveDate < Other.SlotInfoData.LastSaveDate;
	}

	bool operator>(const UDefaultSaveGame& Other) const
	{
		return SlotInfoData.LastSaveDate > Other.SlotInfoData.LastSaveDate;
	}
};
