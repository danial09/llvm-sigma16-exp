//===-- Sigma16SEFrameLowering.h - Sigma1632/64 frame lowering --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEFRAMELOWERING_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEFRAMELOWERING_H

#include "Sigma16FrameLowering.h"

namespace llvm {

class Sigma16SEFrameLowering : public Sigma16FrameLowering {
public:
  explicit Sigma16SEFrameLowering(const Sigma16Subtarget &STI);

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;
};

} // namespace llvm

#endif
