//== Sigma16ISEISelLowering.h - Sigma16ISE DAG Lowering Interface -*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of Sigma16ITargetLowering specialized for sigma1632/64.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEISELLOWERING_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEISELLOWERING_H

#include "Sigma16ISelLowering.h"
#include "Sigma16RegisterInfo.h"

namespace llvm {
class Sigma16SETargetLowering : public Sigma16TargetLowering {
public:
  explicit Sigma16SETargetLowering(const Sigma16TargetMachine &TM,
                                   const Sigma16Subtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

private:
};
} // namespace llvm

#endif // Sigma16ISEISELLOWERING_H
