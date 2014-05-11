using UnrealBuildTool;
using System.IO;

public class projectzCore : ModuleRules {
    public projectzCore(TargetInfo Target) {
        ModulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));

        PrivateIncludePaths.Add(Path.Combine(ModulePath, "Private"));

        PublicDependencyModuleNames.Add("Core");
    }

    private readonly string ModulePath;
}
