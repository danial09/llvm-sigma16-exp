//===-- Sigma16InstPrinter.cpp - Convert Sigma16 MCInst to assembly syntax -==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an Sigma16 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "Sigma16InstPrinter.h"

#include "Sigma16InstrInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "Sigma16GenAsmWriter.inc"

void Sigma16InstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << StringRef(getRegisterName(RegNo));
}

//@1 {
void Sigma16InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                   StringRef Annot, const MCSubtargetInfo &STI,
                                   raw_ostream &O) {
  // Try to print any aliases first.
  if (!printAliasInstr(MI, Address, O))
    printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void Sigma16InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI, true);
}

void Sigma16InstPrinter::printUnsignedImm(const MCInst *MI, int opNum,
                                          raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(opNum);
  if (MO.isImm())
    O << (unsigned short int)MO.getImm();
  else
    printOperand(MI, opNum, O);
}

void Sigma16InstPrinter::printMemOperand(const MCInst *MI, int opNum,
                                         raw_ostream &O) {
  // Load/Store memory operands -- imm[reg]
  // If PIC target the target is loaded as the
  // pattern load R3,imm[R5]
  printOperand(MI, opNum + 1, O);
  O << "[";
  printOperand(MI, opNum, O);
  O << "]";
}

// #if CH >= CH7_1
//  The DAG data node, mem_ea of Sigma16InstrInfo.td, cannot be disabled by
//  ch7_1, only opcode node can be disabled.
//  void Sigma16InstPrinter::
//  printMemOperandEA(const MCInst *MI, int opNum, raw_ostream &O) {
//    // when using stack locations for not load/store instructions
//    // print the same way as all normal 3 operand instructions.
//    printOperand(MI, opNum, O);
//    O << ",";
//    printOperand(MI, opNum+1, O);
//    return;
//  }
// #endif
