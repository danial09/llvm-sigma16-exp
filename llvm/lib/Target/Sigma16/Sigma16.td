//===-- Sigma16.td - Describe the Sigma16 Target Machine ---------*- tablegen -*-===//
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

include "Sigma16Schedule.td"
include "Sigma16InstrInfo.td"

def Sigma16InstrInfo : InstrInfo;
//===----------------------------------------------------------------------===//
// Sigma16 processors supported.
//===----------------------------------------------------------------------===//

// Will generate Sigma16GenAsmWrite.inc included by Sigma16InstPrinter.cpp, contents 
//  as follows,
// void Sigma16InstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *Sigma16InstPrinter::getRegisterName(unsigned RegNo) {...}
def Sigma16 : Target {
// def Sigma16InstrInfo : InstrInfo as before.
  let InstructionSet = Sigma16InstrInfo;
}

