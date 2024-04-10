// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveSlotData.generated.h"


USTRUCT(BlueprintType)
struct FSaveSlotData
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

	FSaveSlotData()
	{
		CreationDate = FDateTime::Now();
		LastSaveDate = FDateTime::Now();
		TimePlayed = FDateTime::Now();
	}

	bool operator==(const FSaveSlotData& Other) const
	{
		return SlotName == Other.SlotName;
	}
};
