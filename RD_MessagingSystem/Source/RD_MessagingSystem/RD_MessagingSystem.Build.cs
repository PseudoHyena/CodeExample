using System.IO;
using UnrealBuildTool;

public class RD_MessagingSystem : ModuleRules
{
	public RD_MessagingSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		bAddDefaultIncludePaths = false;
		
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "..")));
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				"GameplayTags",
				
				"RD_Generic",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}