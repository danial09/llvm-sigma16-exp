//===- Sigma16MCAsmInfo.cpp - Sigma16 asm properties ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the Sigma16MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "Sigma16MCAsmInfo.h"
namespace llvm {

void Sigma16MCAsmInfo::anchor() {}

Sigma16MCAsmInfo::Sigma16MCAsmInfo(const Triple &TheTriple) {
  CodePointerSize = 2;
  CalleeSaveStackSlotSize = 2;
  CommentString = ";";
  AlignmentIsInBytes = false;
  StackGrowsUp = true;

  // All Sigma16 types are 2 bytes. Additionally, the directive comes
  // after the section, not before.
  HasFunctionAlignment = false;
  UseDotAlignForAlignment = false;
  Data8bitsDirective = "\tdata\t";
  Data16bitsDirective = "\tdata\t";
  HasDotTypeDotSizeDirective = true;
  SupportsDebugInformation = true;
  LabelSuffix = "";
  HasDotTypeDotSizeDirective = false;
  HasIdentDirective = false;
  GlobalDirective = ".export\t";
  AsciiDirective = nullptr;
  AscizDirective = nullptr;
}

} // namespace llvm