add_llvm_component_group(Sigma16)

set(LLVM_TARGET_DEFINITIONS Sigma16.td)

# Generate Sigma16GenRegisterInfo.inc and Sigma16GenInstrInfo.inc which included by
#  your hand code C++ files.
# Sigma16GenRegisterInfo.inc came from Sigma16RegisterInfo.td, Sigma16GenInstrInfo.inc
#  came from Sigma16InstrInfo.td.
tablegen(LLVM Sigma16GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM Sigma16GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM Sigma16GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM Sigma16GenCodeEmitter.inc -gen-emitter)
tablegen(LLVM Sigma16GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM Sigma16GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM Sigma16GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM Sigma16GenCallingConv.inc -gen-callingconv)
tablegen(LLVM Sigma16GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM Sigma16GenMCPseudoLowering.inc -gen-pseudo-lowering)

# Sigma16CommonTableGen must be defined
add_public_tablegen_target(Sigma16CommonTableGen)

# Sigma16CodeGen should match with LLVMBuild.txt Sigma16CodeGen
add_llvm_target(Sigma16CodeGen
        Sigma16AsmPrinter.cpp
        Sigma16FrameLowering.cpp
        Sigma16ISelDAGToDAG.cpp
        Sigma16InstrInfo.cpp
        Sigma16ISelLowering.cpp
        Sigma16MachineFunction.cpp
        Sigma16MCInstLower.cpp
        Sigma16RegisterInfo.cpp
        Sigma16SEFrameLowering.cpp
        Sigma16SEInstrInfo.cpp
        Sigma16SEISelDAGToDAG.cpp
        Sigma16SEISelLowering.cpp
        Sigma16SERegisterInfo.cpp
        Sigma16Subtarget.cpp
        Sigma16TargetMachine.cpp
        Sigma16TargetObjectFile.cpp

        LINK_COMPONENTS
        Analysis
        AsmPrinter
        CodeGen
        Core
        MC
        Sigma16Desc
        Sigma16Info
        SelectionDAG
        Support
        Target
        GlobalISel

        ADD_TO_COMPONENT
        Sigma16
        )

# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
#add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
