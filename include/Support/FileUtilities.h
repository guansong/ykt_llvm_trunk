//===- Support/FileUtilities.h - File System Utilities ----------*- C++ -*-===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
//
// This file defines a family of utility functions which are useful for doing
// various things with files.
//
//===----------------------------------------------------------------------===//

#ifndef SUPPORT_FILEUTILITIES_H
#define SUPPORT_FILEUTILITIES_H

#include <string>

namespace llvm {

/// CheckMagic - Returns true IFF the file named FN begins with Magic. FN must
/// name a readable file.
///
bool CheckMagic (const std::string &FN, const std::string &Magic);

/// IsArchive - Returns true IFF the file named FN appears to be a "ar" library
/// archive. The file named FN must exist.
///
bool IsArchive (const std::string &FN);

/// IsBytecode - Returns true IFF the file named FN appears to be an LLVM
/// bytecode file. The file named FN must exist.
///
bool IsBytecode (const std::string &FN);

/// IsSharedObject - Returns trus IFF the file named FN appears to be a shared
/// object with an ELF header. The file named FN must exist.
///
bool IsSharedObject(const std::string &FN);

/// FileOpenable - Returns true IFF Filename names an existing regular file
/// which we can successfully open.
///
bool FileOpenable (const std::string &Filename);

/// DiffFiles - Compare the two files specified, returning true if they are
/// different or if there is a file error.  If you specify a string to fill in
/// for the error option, it will set the string to an error message if an error
/// occurs, allowing the caller to distinguish between a failed diff and a file
/// system error.
///
bool DiffFiles(const std::string &FileA, const std::string &FileB,
               std::string *Error = 0);


/// MoveFileOverIfUpdated - If the file specified by New is different than Old,
/// or if Old does not exist, move the New file over the Old file.  Otherwise,
/// remove the New file.
///
void MoveFileOverIfUpdated(const std::string &New, const std::string &Old);
 
/// removeFile - Delete the specified file
///
void removeFile(const std::string &Filename);

/// getUniqueFilename - Return a filename with the specified prefix.  If the
/// file does not exist yet, return it, otherwise add a suffix to make it
/// unique.
///
std::string getUniqueFilename(const std::string &FilenameBase);

///
/// Method: MakeFileExecutable()
///
/// Description:
///	This method turns on whatever access attributes are needed to make the
///	specified file executable.
///
/// Return value:
///	True  - The operation succeeded.
///	False - The operation failed.
///
/// Notes:
///	In case of failure, the file's access attributes are unspecified.
///
bool MakeFileExecutable (const std::string & Filename);

///
/// Method: MakeFileReadable()
///
/// Description:
///	This method turns on whatever access attributes are needed to make the
///	specified file readable.
///
/// Return value:
///	True  - The operation succeeded.
///	False - The operation failed.
///
/// Notes:
///	In case of failure, the file's access attributes are unspecified.
///
bool MakeFileReadable (const std::string & Filename);


/// FDHandle - Simple handle class to make sure a file descriptor gets closed
/// when the object is destroyed.  This handle acts similarly to an
/// std::auto_ptr, in that the copy constructor and assignment operators
/// transfer ownership of the handle.  This means that FDHandle's do not have
/// value semantics.
///
class FDHandle {
  int FD;
public:
  FDHandle() : FD(-1) {}
  FDHandle(int fd) : FD(fd) {}
  FDHandle(FDHandle &RHS) : FD(RHS.FD) {
    RHS.FD = -1;       // Transfer ownership
  }

  ~FDHandle() throw();

  /// get - Get the current file descriptor, without releasing ownership of it.
  int get() const { return FD; }
  operator int() const { return FD; }

  FDHandle &operator=(int fd) throw();

  FDHandle &operator=(FDHandle &RHS) {
    int fd = RHS.FD;
    RHS.FD = -1;       // Transfer ownership
    return operator=(fd);
  }

  /// release - Take ownership of the file descriptor away from the FDHandle
  /// object, so that the file is not closed when the FDHandle is destroyed.
  int release() {
    int Ret = FD;
    FD = -1;
    return Ret;
  }
};
} // End llvm namespace

#endif
