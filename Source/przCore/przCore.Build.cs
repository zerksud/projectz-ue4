using UnrealBuildTool;
using System.IO;

public class przCore : ModuleRules {
    public przCore(TargetInfo Target) {
        PublicDependencyModuleNames.Add("Core");

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
    }
}
