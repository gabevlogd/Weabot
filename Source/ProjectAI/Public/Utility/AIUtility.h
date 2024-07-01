// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIUtility.generated.h"

UCLASS()
class PROJECTAI_API UAIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AI Utility")
	static void SetPeripheralVisionAngle (UAIPerceptionComponent* PerceptionComponent, float PeripheralVisionAngle);

	UFUNCTION(BlueprintCallable, Category = "AI Utility")
	static UAISenseConfig* GetSenseConfig(UAIPerceptionComponent* PerceptionComponent, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintCallable, Category = "AI Utility")
	static void SetSightRadius(UAIPerceptionComponent* PerceptionComponent, float SightRadius);

	UFUNCTION(BlueprintCallable, Category = "AI Utility")
	static void SetLoseSightRadius(UAIPerceptionComponent* PerceptionComponent, float LoseSightRadius);
	
	UFUNCTION(BlueprintCallable, Category = "AI Utility")
	static void RequestStimuliListenerUpdate(UAIPerceptionComponent* PerceptionComponent);
};
