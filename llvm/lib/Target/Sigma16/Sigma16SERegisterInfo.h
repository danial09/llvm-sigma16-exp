//===- Sigma16SERegisterInfo.h - Sigma1632 Register Information -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma1632/64 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEREGISTERINFO_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEREGISTERINFO_H

#include "Sigma16RegisterInfo.h"

namespace llvm {
class Sigma16SEInstrInfo;

class Sigma16SERegisterInfo : public Sigma16RegisterInfo {
public:
    Sigma16SERegisterInfo(const Sigma16Subtarget &Subtarget);
    const TargetRegisterClass *intRegClass(unsigned Size) const override;
};

} // end namespace llvm

#endif
