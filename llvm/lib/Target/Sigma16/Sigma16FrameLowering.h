//=-- Sigma16FrameLowering.h - Define frame lowering for Sigma16 -*- C++ -*-=//
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
#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16FRAMELOWERING_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16FRAMELOWERING_H

#include "Sigma16.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class Sigma16Subtarget;

class Sigma16FrameLowering : public TargetFrameLowering {
protected:
    const Sigma16Subtarget &STI;

public:
    explicit Sigma16FrameLowering(const Sigma16Subtarget &sti, unsigned Alignment)
        : TargetFrameLowering(StackGrowsUp, Align(Alignment), 0,
                              Align(Alignment)),
          STI(sti) {}

    static const Sigma16FrameLowering *create(const Sigma16Subtarget &ST);

//  unsigned getStackAlignmentSkew(const MachineFunction &MF) const override;

    bool hasFP(const MachineFunction &MF) const override;
};

/// Create Sigma16FrameLowering objects.
const Sigma16FrameLowering *
createSigma16SEFrameLowering(const Sigma16Subtarget &ST);

} // namespace llvm

#endif
