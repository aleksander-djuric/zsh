/*
 * zsh.h
 *
 * Description: Simple shell in C
 * Copyright (c) 2017 Aleksander Djuric. All rights reserved.
 * Distributed under the GNU Lesser General Public License (LGPL).
 * The complete text of the license can be found in the LICENSE
 * file included in the distribution.
 *
 */

#ifndef _ZSH_H
#define _ZSH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "config.h"

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "zsh"
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#ifndef NAME_MAX
#define NAME_MAX 255
#endif
#define MAX_ARGS 512

#define IN 0
#define OUT 1

#define DEF_HOME "/"
#define DEF_MAIL "/dev/null"
#define DEF_PATH "/usr/local/bin:/usr/bin:/bin"

#define UCOLOR "\033[01;32m"
#define HCOLOR "\033[01;34m"
#define RESET "\033[00m"
#define CLEAR "\033[H\033[J"

#endif // _ZSH_H
