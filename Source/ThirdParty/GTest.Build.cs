using UnrealBuildTool;
using System.IO;

public class GTest : ModuleRules {
    public GTest(TargetInfo Target) {
        Type = ModuleType.External;

        ModulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
        GTestPath = Path.Combine(ModulePath, "../../../gtest/");

        if (Target.Platform == UnrealTargetPlatform.Win64) {
            string GTestIncludePath = Path.Combine(GTestPath, "include/");
            PublicIncludePaths.Add(GTestIncludePath);

            string GTestLibPath = Path.Combine(GTestPath, "lib/gtest.lib");
            PublicAdditionalLibraries.Add(GTestLibPath);
        }
    }

    private readonly string ModulePath;
    private readonly string GTestPath;
}
