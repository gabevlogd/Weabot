// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generic/SaveSystem/Data/Saves/SlotInfoItem.h"
#include "SlotInfoData.generated.h"


USTRUCT(BlueprintType)
struct FSlotInfoData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SlotInfoName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime LastSaveDate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double TimePlayed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USlotInfoItem* SlotInfoItem;

	FSlotInfoData()
	{
		SlotInfoItem = nullptr;
		LastSaveDate = FDateTime::Now();
		TimePlayed = 0;
	}

	explicit FSlotInfoData(const FString& SlotName)
	{
		SlotInfoItem = nullptr;
		SlotInfoName = SlotName;
		FSlotInfoData();
		TimePlayed = 0;
	}

	bool operator==(const FSlotInfoData& Other) const
	{
		return SlotInfoName == Other.SlotInfoName;
	}

	bool operator<(const FSlotInfoData& Other) const
	{
		return LastSaveDate < Other.LastSaveDate;
	}

	bool operator>(const FSlotInfoData& Other) const
	{
		return LastSaveDate > Other.LastSaveDate;
	}
};
