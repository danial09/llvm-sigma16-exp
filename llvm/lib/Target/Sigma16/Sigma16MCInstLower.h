//===-- Sigma16MCInstLower.h - Lower MachineInstr to MCInst ----*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16MCINSTLOWER_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16MCINSTLOWER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class Sigma16AsmPrinter;

//@1 {
/// This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY Sigma16MCInstLower {
    //@2
    typedef MachineOperand::MachineOperandType MachineOperandType;
    MCContext *Ctx;
    Sigma16AsmPrinter &AsmPrinter;

public:
    Sigma16MCInstLower(Sigma16AsmPrinter &Asmprinter);
    void initialize(MCContext *C);
    void lower(const MachineInstr *MI, MCInst &OutMI) const;
    MCOperand lowerOperand(const MachineOperand &MO, unsigned offset = 0) const;
};
} // namespace llvm

#endif
