#ifndef AVIContainer_H
#define AVIContainer_H

#include <fstream>
#include "SimpleTree.h"

class AVIContainer
{
private:
	Node* _rootNode{};	// Usually RIFF header
	unsigned int _fileSize;

	unsigned int CreateRIFFHeader(void* buffer, unsigned offset);
	unsigned int CreateHDRLHeader(void* buffer, unsigned offset);
	unsigned int CreateAVIMainHeader(void* buffer, unsigned offset);
	unsigned int CreateStreamHeader(void* buffer, unsigned offset);
	unsigned CreateMOVIHeader(void* buffer, unsigned offset);
	unsigned int CreateAVIStreamHeader(void* buffer, unsigned offset);
	unsigned CreateBitmapInfoHeader(void* buffer, unsigned offset);

	int WriteHeaderSequence();

	uint8_t* _dataBuffer;

	unsigned int _width;
	unsigned int _height;
	unsigned int _framesPerSecond;
	unsigned int _frameCount;

	// Pre-calculated sizes
	unsigned int _frameSize;

public:
	bool BuildAVI(uint8_t* dataBuffer, Node* node);
	AVIContainer(int width, int height, int framesPerSecond, int frameCount);
	//AVIContainer(const char *, avi_usersettings settings);

	~AVIContainer();

	void SetFourCC(uint32_t* fourCC, const char value[]);

	//void SetFourCC(uint32_t* fcc, std::string str);

	//void SetFourCC(uint32_t* fcc, int hexval);

	unsigned AddFrame(void* dataBuffer, unsigned offset);

	void* GetFileData(unsigned int& offset, unsigned int blockSize);

	void WriteToFile(std::string filePath);
};

#endif //AVIContainer_H
