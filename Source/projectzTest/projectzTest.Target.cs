using UnrealBuildTool;
using System.Collections.Generic;

public class projectzTestTarget : TargetRules {
    public projectzTestTarget(TargetInfo Target) {
        Type = TargetType.Program;
    }

    //
    // TargetRules interface.
    //

    public override void SetupBinaries(
        TargetInfo Target,
        ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
        ref List<string> OutExtraModuleNames) {

        OutBuildBinaryConfigurations.Add(new UEBuildBinaryConfiguration(
            InType: UEBuildBinaryType.Executable,
            InModuleNames: new List<string> { "projectzTest" }));
    }

    public override bool GetSupportedPlatforms(ref List<UnrealTargetPlatform> OutPlatforms) {
        OutPlatforms.Add(UnrealTargetPlatform.Win64);

        return true;
    }

    public override bool ShouldCompileMonolithic(UnrealTargetPlatform InPlatform, UnrealTargetConfiguration InConfiguration) {
        return true;
    }

    public override void SetupGlobalEnvironment(
        TargetInfo Target,
        ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
        ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration) {

        UEBuildConfiguration.bCompileNetworkProfiler = false;
        UEBuildConfiguration.bCompileLeanAndMeanUE = true;
        UEBuildConfiguration.bBuildEditor = false;
        UEBuildConfiguration.bCompileAgainstEngine = false;
        UEBuildConfiguration.bCompileAgainstCoreUObject = false;

        OutLinkEnvironmentConfiguration.bIsBuildingConsoleApplication = true;
    }
}
