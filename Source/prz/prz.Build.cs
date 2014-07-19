using UnrealBuildTool;
using System.IO;

public class prz : ModuleRules {
    public prz(TargetInfo Target) {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "przCore" });

        string modulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
        PrivateIncludePaths.Add(Path.Combine(modulePath, "Private"));
    }
}
