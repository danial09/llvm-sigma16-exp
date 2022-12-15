//===-- Sigma16RegisterInfo.h - Sigma16 Register Information Impl --*- C++ -*=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16REGISTERINFO_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16REGISTERINFO_H

#include "Sigma16.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "Sigma16GenRegisterInfo.inc"

namespace llvm {
class Sigma16Subtarget;
class TargetInstrInfo;
class Type;

class Sigma16RegisterInfo : public Sigma16GenRegisterInfo {
protected:
  const Sigma16Subtarget &Subtarget;

public:
  Sigma16RegisterInfo(const Sigma16Subtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

  /// Stack Frame Processing Methods
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// Debug information queries.
  Register getFrameRegister(const MachineFunction &MF) const override;

  Register getStackRegister() const;

  /// \brief Return GPR register class.
  virtual const TargetRegisterClass *intRegClass(unsigned Size) const = 0;
};

} // end namespace llvm

#endif
