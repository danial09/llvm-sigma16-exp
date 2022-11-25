//=-- llvm/Target/Sigma16TargetObjectFile.h - Sigma16 Object Info -*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16TARGETOBJECTFILE_H

#include "Sigma16TargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class Sigma16TargetMachine;
class Sigma16TargetObjectFile : public TargetLoweringObjectFileELF {
  MCSection *SmallDataSection;
  MCSection *SmallBSSSection;
  const Sigma16TargetMachine *TM;

public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // end namespace llvm

#endif
