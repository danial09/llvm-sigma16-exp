# MCTargetDesc/CMakeLists.txt
add_llvm_component_library(LLVMSigma16Desc
        Sigma16ABIInfo.cpp
        Sigma16AsmBackend.cpp
        Sigma16ELFObjectWriter.cpp
        Sigma16CodeEmitter.cpp
        Sigma16InstPrinter.cpp
        Sigma16MCAsmInfo.cpp
        Sigma16MCExpr.cpp
        Sigma16MCTargetDesc.cpp
        Sigma16TargetStreamer.cpp

        LINK_COMPONENTS
        MC
        Sigma16Info
        Support

        ADD_TO_COMPONENT
        Sigma16
        )
