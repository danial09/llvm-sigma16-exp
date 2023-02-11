//===-- Sigma16TargetMachine.cpp - Define TargetMachine for Sigma16 -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Sigma16 target spec.
//
//===----------------------------------------------------------------------===//

#include "Sigma16TargetMachine.h"
#include "Sigma16.h"
#include "Sigma16TargetObjectFile.h"

#include "Sigma16SEISelDAGToDAG.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16"

extern "C" void LLVMInitializeSigma16Target() {
  RegisterTargetMachine<Sigma16TargetMachine> X(TheSigma16Target);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  return "E-m:e-p:16:16-i32:16-i64:16-a:16-n8:16-S16";
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           std::optional<Reloc::Model> RM) {
  if (!RM.has_value() || JIT)
    return Reloc::Static;
  return *RM;
}

// DataLayout --> Big-endian, 16-bit pointer/ABI/alignment
// The stack is always 16 byte aligned
// On function prologue, the stack is created by decrementing
// its pointer. Once decremented, all references are done with positive
// offset from the stack/frame pointer, using StackGrowsUp enables
// an easier handling.<
// Using CodeModel::Large enables different CALL behavior.
Sigma16TargetMachine::Sigma16TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           std::optional<Reloc::Model> RM,
                                           std::optional<CodeModel::Model> CM,
                                           CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<Sigma16TargetObjectFile>()),
      ABI(Sigma16ABIInfo::computeTargetABI()),
      DefaultSubtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

const Sigma16Subtarget *
Sigma16TargetMachine::getSubtargetImpl(const Function &F) const {
  std::string CPU = TargetCPU;
  std::string FS = TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<Sigma16Subtarget>(TargetTriple, CPU, FS, *this);
  }
  return I.get();
}

namespace {
//@Sigma16PassConfig {
/// Sigma16 Code Generator Pass Configuration Options.
class Sigma16PassConfig : public TargetPassConfig {
public:
  Sigma16PassConfig(Sigma16TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  Sigma16TargetMachine &getSigma16TargetMachine() const {
    return getTM<Sigma16TargetMachine>();
  }

  const Sigma16Subtarget &getSigma16Subtarget() const {
    return *getSigma16TargetMachine().getSubtargetImpl();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *Sigma16TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new Sigma16PassConfig(*this, PM);
}

bool Sigma16PassConfig::addInstSelector() {
  addPass(createSigma16SEISelDag(getSigma16TargetMachine(), getOptLevel()));
  return false;
}
