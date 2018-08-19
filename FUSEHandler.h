#ifndef FUSEHANDLER_H
#define FUSEHANDLER_H

#include <cstdio>
#include <cstring>
#include <system_error>

#define FUSE_USE_VERSION 26
#include <fuse/fuse.h>

#include "DataProvider/AVIContainer.h"

char filename[32] = "/test.avi";
char filename2[32] = "test.avi";

//AVIContainer aviContainer;

static int getattr_callback(const char* path, struct stat* stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));

	if (strcmp(path, "/") == 0)
	{
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}

	if (strcmp(path, filename) == 0)
	{
		stbuf->st_mode = S_IFREG | 0777;
		stbuf->st_nlink = 1;
		stbuf->st_size = 4096; // strlen("123");
		return 0;
	}

	return -ENOENT;
}

static int read_callback(const char* path, char* buf, size_t size, off_t offset, fuse_file_info* fi)
{
	if (strcmp(path, filename) == 0)
	{
		//void* data = aviContainer.GetFileData(offset, size);
		//memcpy(buf, data, size);
		/*size_t len = strlen("123");
		if (offset >= len)
		{
			return 0;
		}

		if (offset + size > len)
		{
			memcpy(buf, "123" + offset, len - offset);
			return len - offset;
		}*/

		//memcpy(buf, "123" + offset, size);
		return size;
	}

	return -ENOENT;
}

static int readdir_callback(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info* fi)
{
	(void)offset;
	(void)fi;

	filler(buf, ".", nullptr, 0);
	filler(buf, "..", nullptr, 0);

	filler(buf, filename2, nullptr, 0);

	return 0;
}

int open_callback(const char* path, fuse_file_info* fi)
{
	return 0;
}

#endif //FUSEHANDLER_H
