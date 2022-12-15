# llvm 10.0.0 change from add_llvm_library to add_llvm_component_library
add_llvm_component_library(LLVMSigma16Info
        Sigma16TargetInfo.cpp

        LINK_COMPONENTS
        Support

        ADD_TO_COMPONENT
        Sigma16
        )
