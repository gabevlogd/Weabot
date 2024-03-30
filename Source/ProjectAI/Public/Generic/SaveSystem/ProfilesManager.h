// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DefaultSaveGame.h"
#include "Data/Structs/ProfileData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProfilesManager.generated.h"


UCLASS()
class PROJECTAI_API UProfilesManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TArray<FProfileData> Profiles;
	static FProfileData CurrentProfile;

public:
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void Init();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectProfile(const FString& ProfileName);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static FProfileData GetCurrentProfile();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void CreateProfile(const FString& ProfileName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void DeleteProfile(FProfileData Profile);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void DeleteAllProfiles();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static TArray<FProfileData> GetProfiles();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FProfileData GetProfileByName(const FString& ProfileName, bool& bExists);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesProfileExist(const FString& ProfileName, FProfileData& OutProfile);

	static void SaveProfiles(UDefaultSaveGame* SaveGameData);
	static void LoadProfiles(const UDefaultSaveGame* SaveGameData);
};
