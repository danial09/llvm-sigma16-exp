//===-- Sigma16.td - Describe the Sigma16 Target Machine ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

include "Sigma16RegisterInfo.td"
include "Sigma16Schedule.td"
include "Sigma16InstrInfo.td"
include "Sigma16CallingConv.td"

def Sigma16InstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Sigma16 Subtarget features                                                    //
//===----------------------------------------------------------------------===//

def FeatureSigma16      : SubtargetFeature<"sigma16", "Sigma16ArchVersion", "Sigma16",
                                "Sigma16 ISA Support",
                                []>;

//===----------------------------------------------------------------------===//
// Sigma16 processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, Sigma16GenericItineraries, Features>;

def : Proc<"sigma16", [FeatureSigma16]>;

// Will generate Sigma16GenAsmWrite.inc included by Sigma16InstPrinter.cpp, contents 
//  as follows,
// void Sigma16InstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *Sigma16InstPrinter::getRegisterName(unsigned RegNo) {...}
def Sigma16 : Target {
// def Sigma16InstrInfo : InstrInfo as before.
  let InstructionSet = Sigma16InstrInfo;
}

