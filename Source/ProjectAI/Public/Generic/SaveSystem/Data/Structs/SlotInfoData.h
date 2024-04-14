// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	FDateTime TimePlayed;

	FSlotInfoData()
	{
		CreationDate = FDateTime::Now();
		LastSaveDate = FDateTime::Now();
		TimePlayed = FDateTime::MinValue();
	}

	bool operator==(const FSlotInfoData& Other) const
	{
		return SlotName == Other.SlotName;
	}
};
