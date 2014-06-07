using UnrealBuildTool;

public class prz : ModuleRules {
    public prz(TargetInfo Target) {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "przCore" });
    }
}
