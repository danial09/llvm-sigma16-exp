add_llvm_component_library(LLVMSigma16AsmPrinter
  Sigma16InstPrinter.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  Sigma16
  )

