//===-- Sigma16InstrInfo.h - Sigma16 Instruction Information ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16INSTRINFO_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16INSTRINFO_H

#include "Sigma16.h"
#include "Sigma16RegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "Sigma16GenInstrInfo.inc"

namespace llvm {

class Sigma16InstrInfo : public Sigma16GenInstrInfo {
  virtual void anchor();

protected:
  const Sigma16Subtarget &Subtarget;

public:
  explicit Sigma16InstrInfo(const Sigma16Subtarget &STI);

  static const Sigma16InstrInfo *create(Sigma16Subtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  virtual const Sigma16RegisterInfo &getRegisterInfo() const = 0;

  /// Return the number of bytes of code the specified instruction may be.
  unsigned getInstSizeInBytes(const MachineInstr &MI) const;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI, Register SrcReg,
                           bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

protected:
  MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                                   MachineMemOperand::Flags Flags) const;
};
const Sigma16InstrInfo *createSigma16SEInstrInfo(const Sigma16Subtarget &STI);
} // namespace llvm

#endif
