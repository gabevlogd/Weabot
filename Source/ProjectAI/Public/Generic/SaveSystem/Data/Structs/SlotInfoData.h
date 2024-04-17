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
	FString SlotName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime CreationDate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime LastSaveDate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double TimePlayed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USlotInfoItem* SlotInfoItem;

	FSlotInfoData()
	{
		SlotInfoItem = nullptr;
		CreationDate = FDateTime::Now();
		LastSaveDate = FDateTime::Now();
		TimePlayed = 0;
	}

	explicit FSlotInfoData(const FString& Name)
	{
		SlotInfoItem = nullptr;
		SlotName = Name;
		FSlotInfoData();
		TimePlayed = 0;
	}

	bool operator==(const FSlotInfoData& Other) const
	{
		return SlotName == Other.SlotName;
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
