using UnrealBuildTool;
using System.IO;

public class projectzTest : ModuleRules {
    public projectzTest(TargetInfo Target) {
        ModulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));

        string UESourcePath = Path.Combine(ModulePath, "../../../unreal_engine/src/Engine/Source/");

        PublicIncludePaths.Add(Path.Combine(UESourcePath, "Runtime/Launch/Public/"));
        PrivateIncludePaths.Add(Path.Combine(UESourcePath, "Runtime/Launch/Private/"));

        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("Projects");

        LinkWithGTest(Target);
    }

    private void LinkWithGTest(TargetInfo Target) {
        if (Target.Platform == UnrealTargetPlatform.Win64) {
            string GTestIncludePath = Path.Combine(ModulePath, "../../../gtest/include/");
            PublicIncludePaths.Add(GTestIncludePath);

            string GTestLibPath = Path.Combine(ModulePath, "../../../gtest/lib/gtest.lib");
            PublicAdditionalLibraries.Add(GTestLibPath);
        }
    }

    private readonly string ModulePath;
}
