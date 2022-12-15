//===-- Sigma16AsmPrinter.cpp - Sigma16 LLVM assembly writer --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format Sigma16 assembly language.
//
//===----------------------------------------------------------------------===//

#include "Sigma16AsmPrinter.h"

#include "MCTargetDesc/Sigma16BaseInfo.h"
#include "MCTargetDesc/Sigma16InstPrinter.h"
#include "Sigma16.h"
#include "Sigma16InstrInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Twine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16-asm-printer"

bool Sigma16AsmPrinter::runOnMachineFunction(MachineFunction &MF) {
    Subtarget = &MF.getSubtarget<Sigma16Subtarget>();
    Sigma16FI = MF.getInfo<Sigma16FunctionInfo>();
    return AsmPrinter::runOnMachineFunction(MF);
}

void Sigma16AsmPrinter::emitInstruction(const MachineInstr *MI) {
    //@EmitInstruction body {
    if (MI->isDebugValue()) {
        SmallString<128> Str;
        raw_svector_ostream OS(Str);

        PrintDebugValueComment(MI, OS);
        return;
    }

    //@print out instruction:
    //  Print out both ordinary instruction and boudle instruction
    MachineBasicBlock::const_instr_iterator I = MI->getIterator();
    MachineBasicBlock::const_instr_iterator E = MI->getParent()->instr_end();

    do {

        if (I->isPseudo())
            llvm_unreachable("Pseudo opcode found in emitInstruction()");

        MCInst TmpInst0;
        MCInstLowering.lower(&*I, TmpInst0);
        OutStreamer->emitInstruction(TmpInst0, getSubtargetInfo());
    } while ((++I != E) && I->isInsideBundle()); // Delay slot check
}

void Sigma16AsmPrinter::printSavedRegsBitmask(raw_ostream &O) {
    // In Sigma16, all registers apart from R0 and R15 are saved.
    O << "0x7ffe";
}

void Sigma16AsmPrinter::emitFrameDirective() {
    // Sigma16 doesn't have a frame directive.
}

void Sigma16AsmPrinter::PrintDebugValueComment(const MachineInstr *MI,
        raw_ostream &OS) {
    // TODO: implement this
    OS << "PrintDebugValueComment()";
}

extern "C" void LLVMInitializeSigma16AsmPrinter() {
    //  RegisterAsmPrinter<Sigma16AsmPrinter> X(TheSigma16Target);
}
