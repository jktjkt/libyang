/**
 * @file faux_mmap.c
 * @brief ly_mmap() and ly_munmap()
 *
 * Copyright (c) 2015-2021 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include "common.h"

#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef HAVE_MMAP
LY_ERR
ly_mmap(struct ly_ctx *ctx, int fd, size_t *length, void **addr)
{
    struct stat sb;
    size_t m;

    assert(length);
    assert(addr);
    assert(fd >= 0);

    if (fstat(fd, &sb) == -1) {
        LOGERR(ctx, LY_ESYS, "Failed to stat the file descriptor (%s) for the mmap().", strerror(errno));
        return LY_ESYS;
    }
    if (!S_ISREG(sb.st_mode)) {
        LOGERR(ctx, LY_EINVAL, "File to mmap() is not a regular file.");
        return LY_ESYS;
    }
    if (!sb.st_size) {
        *addr = NULL;
        return LY_SUCCESS;
    }
    /* On Windows, the mman-win32 mmap() emulation uses CreateFileMapping and MapViewOfFile, and these functions
     * do not allow mapping more than "length of file" bytes for PROT_READ. Remapping existing mappings is not allowed, either.
     * At that point the path of least resistance is just reading the file in as-is. */
    m = sb.st_size + 1;
    char *buf = calloc(m, 1);
    if (!buf) {
        LOGERR(ctx, LY_ESYS, "ly_mmap: malloc() failed (%s).", strerror(errno));
    }
    *addr = buf;
    *length = m;

    lseek(fd, 0, SEEK_SET);
    ssize_t to_read = m - 1;
    while (to_read > 0) {
        ssize_t n = read(fd, buf, to_read);
        if (n == 0) {
            return LY_SUCCESS;
        } else if (n < 0) {
            if (errno == EINTR) {
                continue; // can I get this on Windows?
            }
            LOGERR(ctx, LY_ESYS, "ly_mmap: read() failed (%s).", strerror(errno));
        }
        to_read -= n;
        buf += n;
    }
    return LY_SUCCESS;
}

LY_ERR
ly_munmap(void *addr, size_t length)
{
    (void)length;
    free(addr);
    return LY_SUCCESS;
}
#endif
