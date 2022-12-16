//===-- Sigma16MCTargetDesc.cpp - Sigma16 Target Descriptions -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Sigma16 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "Sigma16MCTargetDesc.h"
#include "Sigma16InstPrinter.h"
#include "Sigma16MCAsmInfo.h"
#include "Sigma16TargetStreamer.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "Sigma16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "Sigma16GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "Sigma16GenRegisterInfo.inc"
#include "Sigma16MCAsmInfo.h"

static std::string selectSigma16ArchFeature(const Triple &TT, StringRef CPU) {
  std::string ArchFeature = "+sigma16";
  return ArchFeature;
}

static MCInstrInfo *createSigma16MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSigma16MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSigma16MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSigma16MCRegisterInfo(X, Sigma16::R0);
  return X;
}

static MCSubtargetInfo *
createSigma16MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string ArchFS = selectSigma16ArchFeature(TT, CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty())
      ArchFS = ArchFS + "," + FS.str();
    else
      ArchFS = FS.str();
  }

  return createSigma16MCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, ArchFS);
}

static MCAsmInfo *createSigma16MCAsmInfo(const MCRegisterInfo &MRI,
                                         const Triple &TT,
                                         const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new Sigma16MCAsmInfo(TT);

  return MAI;
}

static MCInstPrinter *createSigma16MCInstPrinter(const Triple &T,
                                                 unsigned SyntaxVariant,
                                                 const MCAsmInfo &MAI,
                                                 const MCInstrInfo &MII,
                                                 const MCRegisterInfo &MRI) {
  return new Sigma16InstPrinter(MAI, MII, MRI);
}

namespace {
class Sigma16InstrAnalysis : public MCInstrAnalysis {
public:
  Sigma16InstrAnalysis(const MCInstrInfo *Info) : MCInstrAnalysis(Info) {}
};
} // namespace

static MCInstrAnalysis *createSigma16MCInstrAnalysis(const MCInstrInfo *Info) {
  return new Sigma16InstrAnalysis(Info);
}

static MCStreamer *createMCStreamer(const Triple &TT, MCContext &Context,
                                    std::unique_ptr<MCAsmBackend> &&MAB,
                                    std::unique_ptr<MCObjectWriter> &&OW,
                                    std::unique_ptr<MCCodeEmitter> &&Emitter,
                                    bool RelaxAll) {
  return createELFStreamer(Context, std::move(MAB), std::move(OW),
                           std::move(Emitter), RelaxAll);
  ;
}

static MCTargetStreamer *
createSigma16AsmTargetStreamer(MCStreamer &S, formatted_raw_ostream &OS,
                               MCInstPrinter *InstPrint, bool isVerboseAsm) {
  return new Sigma16TargetAsmStreamer(S, OS);
}

//@2 {
extern "C" void LLVMInitializeSigma16TargetMC() {
  for (Target *T : {&TheSigma16Target}) {
    // Register the MC asm info.
    RegisterMCAsmInfoFn X(*T, createSigma16MCAsmInfo);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createSigma16MCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createSigma16MCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T, createSigma16MCSubtargetInfo);

    // Register the MC instruction analyzer.
    TargetRegistry::RegisterMCInstrAnalysis(*T, createSigma16MCInstrAnalysis);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createSigma16MCInstPrinter);

    // TODO: Create the following functions:
    // Register the MC code emitter
    // TargetRegistry::RegisterMCCodeEmitter(*T, createSigma16MCCodeEmitter);

    //     Register the asm backend
    TargetRegistry::RegisterMCAsmBackend(*T, createSigma16AsmBackend);
  }
}
//@2 }
