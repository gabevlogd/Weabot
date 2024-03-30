// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/ProfileData.h"
#include "GameFramework/SaveGame.h"
#include "DefaultSaveGame.generated.h"


UCLASS()
class PROJECTAI_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	FString LastProfileName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save System")
	TArray<FProfileData> Profiles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save System")
	TMap<FName, FTransform> ActorTransforms;
};
