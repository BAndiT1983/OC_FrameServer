#ifndef FUSEHANDLER_H
#define FUSEHANDLER_H

#define FUSE_USE_VERSION 26
#include <fuse/fuse.h>

#include "DataProvider/AVIContainer.h"

void SetAVIContainer(AVIContainer* container);

int getattr_callback(const char *path, struct stat *stbuf);

int read_callback(const char *path, char *buf, size_t size, off_t offset, fuse_file_info *fi);

int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi);

int open_callback(const char *path, fuse_file_info *fi);

#endif //FUSEHANDLER_H
