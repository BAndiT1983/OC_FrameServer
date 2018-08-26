#include "FUSEHandler.h"

#define UNUSED(expr) (void)(expr)

char filename[32] = "/test.avi";
char filename2[32] = "test.avi";

AVIContainer *aviContainer;
void *data;
unsigned int dataSize;

void SetAVIContainer(AVIContainer *container)
{
	aviContainer = container;
	data = aviContainer->GetFileData(dataSize, 12);
}

int getattr_callback(const char *path, struct stat *stbuf)
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
        stbuf->st_size = dataSize; // strlen("123");
		return 0;
	}

	return -ENOENT;
}

//unsigned int currentOffset = 0;

int read_callback(const char *path, char *buf, size_t size, off_t offset, fuse_file_info *fi)
{
	//UNUSED(offset);
	UNUSED(fi);
	//UNUSED(buf);
	//UNUSED(size);

	if (strcmp(path, filename) != 0)
	{
		return -ENOENT;
	}

	// if (size > offset_size)
	// {
	// 	size = offset_size;
	// }

    //currentOffset += offset;

	std::cout << "INFO: data size - " << std::to_string(dataSize) << " | curr offset - " << std::to_string(offset) << std::endl;
	std::cout << "READ: size - " << std::to_string(size) << " | offset - " << std::to_string(offset) << std::endl;

	unsigned int sizeToRead = size;

    if ((offset + size) > dataSize)
	{
        sizeToRead = dataSize - offset;
		std::cout << "FILE_END: " << "ret_size" << std::to_string(sizeToRead) << std::endl;
	}

    memcpy(buf, (uint8_t *)data + offset, sizeToRead);

	return sizeToRead;
}

int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi)
{
	UNUSED(path);
	UNUSED(offset);
	UNUSED(fi);

	filler(buf, ".", nullptr, 0);
	filler(buf, "..", nullptr, 0);

	if (strcmp(path, "/") == 0)
	{
		filler(buf, filename2, nullptr, 0);
	}

	return 0;
}

int open_callback(const char *path, fuse_file_info *fi)
{
	UNUSED(path);
	UNUSED(fi);

	return 0;
}
