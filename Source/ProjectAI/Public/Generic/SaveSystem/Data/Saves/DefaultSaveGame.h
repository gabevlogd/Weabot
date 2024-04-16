// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Generic/SaveSystem/Data/Structs/SlotInfoData.h"
#include "DefaultSaveGame.generated.h"


UCLASS(BlueprintType, Blueprintable)
class PROJECTAI_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	FName SlotInfoName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame()
	{
		SlotInfoName = "";
		ActorTransforms = TMap<FName, FTransform>();
	}

	void SetSlotName(const FName& NewSlotInfoName)
	{
		SlotInfoName = NewSlotInfoName;
	}
};
