#ifndef AVIContainer_H
#define AVIContainer_H

#include <fstream>
#include "SimpleTree.h"

class AVIContainer
{
private:
	/*char *output_filename{}, *hdrBuffer{};
	unsigned int hdrBufferLen{};
	std::fstream file{};*/
	
	Node* _rootNode{};	// Usually RIFF header
	unsigned int _fileSize;

	//avi_usersettings usersettings;

	//uint32_t calculateSize(sizeType type);

	unsigned int CreateRIFFHeader(void* buffer, unsigned offset);
	unsigned int CreateHDRLHeader(void* buffer, unsigned offset);
	unsigned int CreateAVIMainHeader(void* buffer, unsigned offset);
	unsigned int CreateStreamHeader(void* buffer, unsigned offset);
	unsigned CreateMOVIHeader(void* buffer, unsigned offset);
	unsigned int CreateAVIStreamHeader(void* buffer, unsigned offset);
	unsigned CreateBitmapInfoHeader(void* buffer, unsigned offset);

	//avi_list_h hdrlHeader();

	//AVIMAINHEADER avihHeader();

	//avi_list_h strlHeader();

	//AVISTREAMHEADER strhHeader();

	//avi_chunk_h strfHeader_c();

	//BITMAPINFOHEADER strfHeader_v();

	//avi_list_h moviHeader();

	int WriteHeaderSequence();


	void init_sizes();

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

	void SetFourCC(uint32_t* fourCC, char value[]);

	//void SetFourCC(uint32_t* fcc, std::string str);

	//void SetFourCC(uint32_t* fcc, int hexval);

	unsigned AddFrame(void* dataBuffer, unsigned offset);

	void* GetFileData(unsigned int offset, unsigned int blockSize);

	void WriteToFile(std::string filePath);
};

#endif //AVIContainer_H
