// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LudumDare56 : ModuleRules
{
	public LudumDare56(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
