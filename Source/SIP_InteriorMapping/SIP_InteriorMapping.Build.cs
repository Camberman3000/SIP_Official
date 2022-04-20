// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SIP_InteriorMapping : ModuleRules
{
	public SIP_InteriorMapping(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
