//===-- Sigma16SEInstrInfo.h - Sigma1632/64 Instruction Information ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma1632/64 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEINSTRINFO_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEINSTRINFO_H


#include "Sigma16InstrInfo.h"
#include "Sigma16SERegisterInfo.h"
#include "Sigma16MachineFunction.h"

namespace llvm {

class Sigma16SEInstrInfo : public Sigma16InstrInfo {
  const Sigma16SERegisterInfo RI;

public:
  explicit Sigma16SEInstrInfo(const Sigma16Subtarget &STI);

  const Sigma16RegisterInfo &getRegisterInfo() const override;

  
};

}

#endif

