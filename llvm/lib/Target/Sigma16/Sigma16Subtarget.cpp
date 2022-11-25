//===-- Sigma16Subtarget.cpp - Sigma16 Subtarget Information --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Sigma16 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "Sigma16Subtarget.h"

#include "Sigma16.h"
#include "Sigma16MachineFunction.h"
#include "Sigma16RegisterInfo.h"

#include "Sigma16TargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "Sigma16GenSubtargetInfo.inc"

extern bool FixGlobalBaseReg;

void Sigma16Subtarget::anchor() {}

//@1 {
Sigma16Subtarget::Sigma16Subtarget(const Triple &TT, StringRef CPU,
                                   StringRef FS,
                                   const Sigma16TargetMachine &Tm)
    : //@1 }
      // Sigma16GenSubtargetInfo will display features by llc -march=sigma16
      // -mcpu=help
      Sigma16GenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), TM(Tm),
      TargetTriple(TT), TSInfo(),
      InstrInfo(Sigma16InstrInfo::create(
          initializeSubtargetDependencies(CPU, FS, TM))),
      FrameLowering(Sigma16FrameLowering::create(*this)),
      TLInfo(Sigma16TargetLowering::create(TM, *this)) {}

bool Sigma16Subtarget::isPositionIndependent() const {
  return TM.isPositionIndependent();
}

Sigma16Subtarget &
Sigma16Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                  const TargetMachine &TM) {
  if (TargetTriple.getArch() == Triple::sigma16) {
    if (CPU.empty() || CPU == "generic") {
      CPU = "sigma16";
    } else if (CPU == "help") {
      CPU = "";
      return *this;
    }
  } else {
    errs() << "!!!Error, TargetTriple.getArch() = " << TargetTriple.getArch()
           << "CPU = " << CPU << "\n";
    exit(0);
  }

  if (CPU == "sigma16")
    Sigma16ArchVersion = Sigma16;

  else {
    errs() << "-mcpu must be empty(default:sigma16), sigma16"
           << "\n";
  }

  // Parse features string.
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);
  // Initialize scheduling itinerary for the specified CPU.
  InstrItins = getInstrItineraryForCPU(CPU);

  return *this;
}

bool Sigma16Subtarget::abiUsesSoftFloat() const {
  //  return TM->Options.UseSoftFloat;
  return true;
}

const Sigma16ABIInfo &Sigma16Subtarget::getABI() const { return TM.getABI(); }
