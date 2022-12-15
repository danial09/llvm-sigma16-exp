//===-- Sigma16TargetStreamer.cpp - Sigma16 Target Streamer Methods -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Sigma16 specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "Sigma16TargetStreamer.h"
#include "Sigma16InstPrinter.h"
#include "Sigma16MCTargetDesc.h"
#include "Sigma16TargetObjectFile.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbolELF.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

Sigma16TargetStreamer::Sigma16TargetStreamer(MCStreamer &S)
    : MCTargetStreamer(S) {}

Sigma16TargetAsmStreamer::Sigma16TargetAsmStreamer(MCStreamer &S,
        formatted_raw_ostream &OS)
    : Sigma16TargetStreamer(S), OS(OS) {}
