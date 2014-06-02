using UnrealBuildTool;
using System.IO;

public class przTest : ModuleRules {
    public przTest(TargetInfo Target) {
        ModulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));

        string UESourcePath = Path.Combine(ModulePath, "../../../unreal_engine/src/Engine/Source/");

        PublicIncludePaths.Add(Path.Combine(UESourcePath, "Runtime/Launch/Public/"));
        PrivateIncludePaths.Add(Path.Combine(UESourcePath, "Runtime/Launch/Private/"));

        PrivateIncludePaths.Add(Path.Combine(ModulePath, "Private/"));

        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("Projects");
        PrivateDependencyModuleNames.Add("GTest");
        PrivateDependencyModuleNames.Add("przCore");
    }

    private readonly string ModulePath;
}
