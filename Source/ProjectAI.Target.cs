// Copyright The Prototypers, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectAITarget : TargetRules
{
	public ProjectAITarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		
		ExtraModuleNames.AddRange( new string[] { "ProjectAI" } );
	}
}
