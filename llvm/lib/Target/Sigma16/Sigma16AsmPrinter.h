//===-- Sigma16AsmPrinter.h - Sigma16 LLVM assembly printer -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Sigma16 assembly printer class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16ASMPRINTER_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16ASMPRINTER_H

#include "Sigma16MCInstLower.h"
#include "Sigma16MachineFunction.h"
#include "Sigma16Subtarget.h"
#include "Sigma16TargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MCStreamer;
class MachineInstr;
class MachineBasicBlock;
class Module;
class raw_ostream;

class LLVM_LIBRARY_VISIBILITY Sigma16AsmPrinter : public AsmPrinter {

  void EmitInstructionWithMacroNoAT(const MachineInstr *MI);

private:
  // lowerOperand - Convert a MachineOperand into the equivalent MCOperand.
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp);

public:
  const Sigma16Subtarget *Subtarget;
  const Sigma16FunctionInfo *Sigma16FI;
  const Sigma16MCInstLower MCInstLowering;

  explicit Sigma16AsmPrinter(TargetMachine &TM,
                             std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(*this) {
    Subtarget = static_cast<Sigma16TargetMachine &>(TM).getSubtargetImpl();
  }

  StringRef getPassName() const override { return "Sigma16 Assembly Printer"; }

  virtual bool runOnMachineFunction(MachineFunction &MF) override;

  void emitInstruction(const MachineInstr *MI) override;
  void printSavedRegsBitmask(raw_ostream &O);
  void printHex32(unsigned int Value, raw_ostream &O);
  void emitFrameDirective();
  const char *getCurrentABIString() const;
  void emitFunctionEntryLabel() override;
  void emitFunctionBodyStart() override;
  void emitFunctionBodyEnd() override;
  void emitStartOfAsmFile(Module &M) override;
  void PrintDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_SIGMA16_SIGMA16ASMPRINTER_H
