//===-- Sigma16SEISelDAGToDAG.h - A Dag to Dag Inst Selector for Sigma16SE -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of Sigma16DAGToDAGISel specialized for sigma16.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16SEISELDAGTODAG_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16SEISELDAGTODAG_H

#include "Sigma16ISelDAGToDAG.h"

namespace llvm {

class Sigma16SEDAGToDAGISel : public Sigma16DAGToDAGISel {

public:
    explicit Sigma16SEDAGToDAGISel(Sigma16TargetMachine &TM, CodeGenOpt::Level OL)
        : Sigma16DAGToDAGISel(TM, OL) {}

private:

    bool runOnMachineFunction(MachineFunction &MF) override;

    bool trySelect(SDNode *Node) override;

    void processFunctionAfterISel(MachineFunction &MF) override;

    // Insert instructions to initialize the global base register in the
    // first MBB of the function.
    //  void initGlobalBaseReg(MachineFunction &MF);

};

FunctionPass *createSigma16SEISelDag(Sigma16TargetMachine &TM,
                                     CodeGenOpt::Level OptLevel);

} // namespace llvm

#endif

