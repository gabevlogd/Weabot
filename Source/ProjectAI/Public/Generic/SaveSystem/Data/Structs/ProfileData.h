// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProfileData.generated.h"


USTRUCT(BlueprintType)
struct FProfileData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ProfileName = "Default";

	bool operator==(const FProfileData& Other) const
	{
		return ProfileName == Other.ProfileName;
	}
};
