//====-- Sigma16MCAsmInfo.h - Sigma16 asm Info ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the Sigma16MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16MCASMINFO_H
#define LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class Sigma16MCAsmInfo : public MCAsmInfoELF {
    void anchor() override;

public:
    explicit Sigma16MCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif