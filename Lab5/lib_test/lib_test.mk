##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lib_test
ConfigurationName      :=Debug
WorkspacePath          :=D:/KURSER/MaskinOrienteradProgrammering/Lab5
ProjectPath            :=D:/KURSER/MaskinOrienteradProgrammering/Lab5/lib_test
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Landa
Date                   :=02/06/2020
CodeLitePath           :=C:/cseapp/CodeLite
LinkerName             :=$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
SharedObjectLinkerName :=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="lib_test.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=
LinkOptions            :=  -T$(ProjectPath)/md407-ram.x -L$(ARM_V6LIB) -L$(ARM_GCC_V6LIB) -nostartfiles
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gcc $(LibrarySwitch)md407d $(LibrarySwitch)c_nano 
ArLibs                 :=  "gcc" "md407d" "c_nano" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v6-m/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-ar.exe rcu
CXX      := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
CC       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-gcc.exe
CXXFLAGS :=  -g -O0 -W $(Preprocessors)
CFLAGS   :=  -g -O0 -w -mthumb -march=armv6-m  -mfloat-abi=soft -std=c99 $(Preprocessors)
ASFLAGS  := 
AS       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-as.exe


##
## User defined environment variables
##
ARM_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v6-m/nofp
ARM_GCC_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v6-m/nofp
ARM_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/softfp
ARM_GCC_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/softfp
ARM_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/hard
ARM_GCC_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/hard
Objects0=$(IntermediateDirectory)/startup.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	C:\cseapp\CodeLite/tools/gcc-arm/bin/arm-none-eabi-objcopy -S -O srec  ./Debug/lib_test.elf ./Debug/lib_test.s19
	C:\cseapp\CodeLite/tools/gcc-arm/bin/arm-none-eabi-objdump -D -S ./Debug/lib_test.elf > ./Debug/lib_test.dass
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c $(IntermediateDirectory)/startup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/KURSER/MaskinOrienteradProgrammering/Lab5/lib_test/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(DependSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c

$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


