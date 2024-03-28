// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GenericSaveGame.generated.h"


UCLASS()
class PROJECTAI_API UGenericSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;
};
