//=== Sigma16TargetMachine.h - Define TargetMachine for Sigma16 -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Sigma16 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_TARGETMACHINE_H
#define LLVM_LIB_TARGET_SIGMA16_TARGETMACHINE_H

#include "MCTargetDesc/Sigma16ABIInfo.h"
#include "Sigma16Subtarget.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class formatted_raw_ostream;
class Sigma16RegisterInfo;

class Sigma16TargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  // Selected ABI
  Sigma16ABIInfo ABI;
  Sigma16Subtarget DefaultSubtarget;

  mutable StringMap<std::unique_ptr<Sigma16Subtarget>> SubtargetMap;

public:
  Sigma16TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       std::optional<Reloc::Model> RM, std::optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);
  ~Sigma16TargetMachine() override = default;

  const Sigma16Subtarget *getSubtargetImpl() const { return &DefaultSubtarget; }

  const Sigma16Subtarget *getSubtargetImpl(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const Sigma16ABIInfo &getABI() const { return ABI; }
};
} // end namespace llvm

#endif
