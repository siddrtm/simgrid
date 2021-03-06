/* Copyright (c) 2004-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SURF_SURF_PRIVATE_HPP
#define SURF_SURF_PRIVATE_HPP

#include "simgrid/forward.h"
#include "xbt/sysdep.h"

/* Generic functions common to all models */

XBT_PRIVATE FILE* surf_fopen(const char* name, const char* mode);
XBT_PRIVATE std::ifstream* surf_ifsopen(std::string name);

/* The __surf_is_absolute_file_path() returns 1 if
 * file_path is a absolute file path, in the other
 * case the function returns 0.
 */
XBT_PRIVATE int __surf_is_absolute_file_path(const char* file_path);

XBT_PRIVATE void check_disk_attachment();

#endif
