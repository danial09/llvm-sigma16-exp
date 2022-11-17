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
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "Sigma16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "Sigma16GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "Sigma16GenRegisterInfo.inc"

//@2 {
extern "C" void LLVMInitializeSigma16TargetMC() {
    RegisterTargetMachine<Sigma16ebTargetMachine> X(TheSigma16Target);
}
//@2 }

