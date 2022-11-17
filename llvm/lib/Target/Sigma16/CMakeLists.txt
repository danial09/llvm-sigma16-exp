add_llvm_component_group(Sigma16)

set(LLVM_TARGET_DEFINITIONS Sigma16.td)

tablegen(LLVM Sigma16GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM Sigma16GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM Sigma16GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM Sigma16GenMCPseudoLowering.inc -gen-pseudo-lowering)

add_public_tablegen_target(Sigma16CommonTableGen)

add_llvm_target(Sigma16CodeGen
  Sigma16TargetMachine.cpp

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