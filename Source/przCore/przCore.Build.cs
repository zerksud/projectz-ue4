using UnrealBuildTool;
using System.IO;

public class przCore : ModuleRules {
    public przCore(TargetInfo Target) {
        PublicDependencyModuleNames.Add("Core");

        string modulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
        PrivateIncludePaths.Add(Path.Combine(modulePath, "Private"));
    }
}
