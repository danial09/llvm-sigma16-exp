//===-- Sigma16TargetStreamer.h - Sigma16 Target Streamer -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_SIGMA16TARGETSTREAMER_H
#define LLVM_LIB_TARGET_SIGMA16_SIGMA16TARGETSTREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/FormattedStream.h"

namespace llvm {

class Sigma16TargetStreamer : public MCTargetStreamer {
public:
    Sigma16TargetStreamer(MCStreamer &S);
};

// This part is for ascii assembly output
class Sigma16TargetAsmStreamer : public Sigma16TargetStreamer {
    formatted_raw_ostream &OS;
//  void emitFileDirective(StringRef Filename) override;

public:
    Sigma16TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_SIGMA16_SIGMA16TARGETSTREAMER_H