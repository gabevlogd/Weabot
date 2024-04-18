// Copyright The Prototypers, Inc. All Rights Reserved.

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
	FName SlotNameKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame()
	{
		SlotNameKey = "";
		ActorTransforms = TMap<FName, FTransform>();
	}

	void SetSlotNameKey(const FName& Key)
	{
		SlotNameKey = Key;
	}
};
