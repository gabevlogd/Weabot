// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/ProfilesManager.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"


FProfileData UProfilesManager::CurrentProfile;
TArray<FProfileData> UProfilesManager::Profiles;

void UProfilesManager::Init()
{
	Profiles.Empty();
	CurrentProfile = FProfileData();
}

void UProfilesManager::SelectProfile(const FString& ProfileName)
{
	bool bExists = false;
	const FProfileData Profile = GetProfileByName(ProfileName, bExists);
	if (bExists)
		CurrentProfile = Profile;
}

FProfileData UProfilesManager::GetCurrentProfile()
{
	return CurrentProfile; // Return it as a copy to avoid modifying the original
}

void UProfilesManager::CreateProfile(const FString& ProfileName)
{
	if (DoesProfileExist(ProfileName, CurrentProfile)) return;

	FProfileData Profile;
	Profile.ProfileName = ProfileName;
	Profiles.Add(Profile);
}

void UProfilesManager::DeleteProfile(FProfileData Profile)
{
	Profiles.Remove(Profile);
}

void UProfilesManager::DeleteAllProfiles()
{
	Profiles.Empty();
}

TArray<FProfileData> UProfilesManager::GetProfiles()
{
	return Profiles;
}

FProfileData UProfilesManager::GetProfileByName(const FString& ProfileName, bool& bExists)
{
	FProfileData Profile;
	bExists = DoesProfileExist(ProfileName, Profile);
	return Profile;
}

bool UProfilesManager::DoesProfileExist(const FString& ProfileName, FProfileData& OutProfile)
{
	for (FProfileData Profile : Profiles)
	{
		if (Profile.ProfileName == ProfileName)
		{
			OutProfile = Profile;
			return true;
		}
	}

	return false;
}

void UProfilesManager::SaveProfiles(UDefaultSaveGame* SaveGameData)
{
	if (!SaveGameData) return;

	SaveGameData->Profiles = Profiles;
	SaveGameData->LastProfileName = CurrentProfile.ProfileName;
}

void UProfilesManager::LoadProfiles(const UDefaultSaveGame* SaveGameData)
{
	if (!SaveGameData) return;
	
	if (!SaveGameData->Profiles.IsEmpty())
		Profiles = SaveGameData->Profiles;

	if (SaveGameData->LastProfileName.IsEmpty())
	{
		bool bExists = false;
		CurrentProfile = GetProfileByName(SaveGameData->LastProfileName, bExists);
	}
}
