//=- Sigma16SEInstrInfo.h - Sigma16 Instruction Information ------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of the TargetInstrInfo
// class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEINSTRINFO_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEINSTRINFO_H

#include "Sigma16InstrInfo.h"
#include "Sigma16MachineFunction.h"
#include "Sigma16SERegisterInfo.h"

namespace llvm {

class Sigma16SEInstrInfo : public Sigma16InstrInfo {
  const Sigma16SERegisterInfo RI;

public:
  explicit Sigma16SEInstrInfo(const Sigma16Subtarget &STI);

  const Sigma16RegisterInfo &getRegisterInfo() const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override;

private:
  void expandRetLR(MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;
};

} // namespace llvm

#endif
