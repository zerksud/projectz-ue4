using UnrealBuildTool;

public class projectz : ModuleRules {
    public projectz(TargetInfo Target) {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "projectzCore" });
    }
}
