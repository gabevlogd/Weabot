// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once


#define SAVE_EXTENSION TEXT(".sav")
#define SAVES_DIRECTORY TEXT("saves/")
#define SAVES_DIRECTORY_FULLPATH FPaths::Combine(FPaths::ProjectSavedDir(), "SaveGames/", SAVES_DIRECTORY)
#define SAVE_SLOT_NAME TEXT("save_")
#define AUTO_SAVE_SLOT_NAME TEXT("auto_")

// Meta Files
#define META_DIRECTORY TEXT(".meta/")
#define META_DIRECTORY_FULLPATH FPaths::Combine(FPaths::ProjectSavedDir(), META_DIRECTORY)
#define SLOT_INFOS_PATH_NAME FPaths::Combine(META_DIRECTORY, TEXT("infos"))
