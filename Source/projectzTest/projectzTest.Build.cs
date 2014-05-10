using UnrealBuildTool;

public class projectzTest : ModuleRules {
    public projectzTest(TargetInfo Target) {
        const string UESourceFolder = "../../unreal_engine/src/Engine/Source/";

        PublicIncludePaths.Add(UESourceFolder + "Runtime/Launch/Public");
        PrivateIncludePaths.Add(UESourceFolder + "Runtime/Launch/Private");

        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("Projects");
    }
}
