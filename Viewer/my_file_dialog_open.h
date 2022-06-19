// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef FILE_DIALOG_OPEN_H
#define FILE_DIALOG_OPEN_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

  // Returns a string with a path to an existing file
  // The string is returned empty if no file is selected
  // (on Linux machines, it assumes that Zenity is installed)
  //
  // Usage:
  //   std::string str = get_open_file_path();
inline std::string my_file_dialog_open(const std::string& fn_default)
{
  const int FILE_DIALOG_MAX_BUFFER = 1024;
  char buffer[FILE_DIALOG_MAX_BUFFER];
  buffer[0] = '\0';
  buffer[FILE_DIALOG_MAX_BUFFER - 1] = 'x'; // Initialize last character with a char != '\0'

  assert(fn_default.size() < 32);
  std::string fn = "/usr/bin/zenity --file-selection --filename=";
  const size_t sz = fn.size();
  fn.resize(fn.size() + 32);

  std::copy(fn_default.begin(), fn_default.end(), fn.begin() + sz);

  // The zenity executable is assumed to be in the environment's path
  FILE * output = popen(fn.data(),"r");
  if (output)
  {
    auto ret = fgets(buffer, FILE_DIALOG_MAX_BUFFER, output);
    if (ret == NULL || ferror(output))
    {
      // I/O error
      buffer[0] = '\0';
    }
    if (buffer[FILE_DIALOG_MAX_BUFFER - 1] == '\0')
    {
      // File name too long, buffer has been filled, so we return empty string instead
      buffer[0] = '\0';
    }
  }

  // Replace last '\n' by '\0'
  if(strlen(buffer) > 0)
  {
    buffer[strlen(buffer)-1] = '\0';
  }

  return std::string(buffer);
}


#endif
