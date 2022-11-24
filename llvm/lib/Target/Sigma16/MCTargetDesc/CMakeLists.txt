# MCTargetDesc/CMakeLists.txt
add_llvm_component_library(LLVMSigma16Desc
  Sigma16ABIInfo.cpp
  Sigma16MCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Sigma16Info
  Support

  ADD_TO_COMPONENT
  Sigma16
  )

