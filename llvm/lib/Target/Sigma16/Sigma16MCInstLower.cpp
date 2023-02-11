//===-- Sigma16MCInstLower.cpp - Convert Sigma16 MachineInstr to MCInst ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower Sigma16 MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "Sigma16MCInstLower.h"

#include "MCTargetDesc/Sigma16BaseInfo.h"
#include "MCTargetDesc/Sigma16MCExpr.h"
#include "Sigma16AsmPrinter.h"
#include "Sigma16InstrInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

// Sigma16MCInstLower::Sigma16MCInstLower(Sigma16AsmPrinter &Asmprinter)
//     : AsmPrinter(Asmprinter) {}

// void Sigma16MCInstLower::Initialize(MCContext *C) {
//   Ctx = C;
// }

static void createMcInst(MCInst &Inst, unsigned Opc, const MCOperand &Opnd0,
                         const MCOperand &Opnd1,
                         const MCOperand &Opnd2 = MCOperand()) {
  Inst.setOpcode(Opc);
  Inst.addOperand(Opnd0);
  Inst.addOperand(Opnd1);
  if (Opnd2.isValid())
    Inst.addOperand(Opnd2);
}

//@LowerOperand {
MCOperand Sigma16MCInstLower::lowerOperand(const MachineOperand &MO,
                                           unsigned Offset) const {
  MachineOperandType MOTy = MO.getType();

  switch (MOTy) {
  //@2
  default:
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm() + Offset);
  case MachineOperand::MO_RegisterMask:
    break;
  case MachineOperand::MO_GlobalAddress:
    return LowerSymbolOperand(MO, GetGlobalAddressSymbol(MO));
  }

  return MCOperand();
}

void Sigma16MCInstLower::lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned I = 0, E = MI->getNumOperands(); I != E; ++I) {
    const MachineOperand &MO = MI->getOperand(I);
    MCOperand MCOp = lowerOperand(MO);

    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}

MCOperand Sigma16MCInstLower::LowerSymbolOperand(const MachineOperand &MO,
                                                 MCSymbol *Sym) const {
  // FIXME: We would like an efficient form for this, so we don't have to do a
  // lot of extra uniquing.
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, *Ctx);

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case 0:
    break;
  }

  if (!MO.isJTI() && MO.getOffset())
    Expr = MCBinaryExpr::createAdd(
        Expr, MCConstantExpr::create(MO.getOffset(), *Ctx), *Ctx);
  return MCOperand::createExpr(Expr);
}

MCSymbol *
Sigma16MCInstLower::GetGlobalAddressSymbol(const MachineOperand &MO) const {
  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case 0:
    break;
  }

  return AsmPrinter.getSymbol(MO.getGlobal());
}