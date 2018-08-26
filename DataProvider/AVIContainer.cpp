#include "AVIContainer.h"

#include "AVIStruct.h"

#include <cstring>
#include <functional>

#include "SimpleTree.h"
#include <iostream>

// Reference: http://yaai.sourceforge.net/yaai/fileformat.html 
const unsigned int AVIListSize = sizeof(AVIList);
const unsigned int AVIChunkSize = sizeof(AVIChunk);
const unsigned int AVIMainHeaderSize = sizeof(AVIMainHeader); // avih
const unsigned int AVIStreamHeaderSize = sizeof(AVIStreamHeader);
const unsigned int BitmapInfoHeaderSize = sizeof(BitmapInfoHeader);

#define BIND_FUNC(x) std::bind(&AVIContainer::x, this, std::placeholders::_1, std::placeholders::_2)

AVIContainer::AVIContainer(int width, int height, int framesPerSecond, int frameCount) :
	_width(width),
	_height(height),
	_framesPerSecond(framesPerSecond),
	_frameCount(frameCount),
	_frameSize(width * height * 3) // 3 -> RGB
{
	_dataBuffer = new uint8_t[30 * 1024 * 1024]; // 30MB

	_rootNode = new Node(BIND_FUNC(CreateRIFFHeader), true);
	Node* hdrlNode = _rootNode->AddChild(BIND_FUNC(CreateHDRLHeader), true);
	hdrlNode->AddChild(BIND_FUNC(CreateAVIMainHeader), false);
	Node* strlNode = hdrlNode->AddChild(BIND_FUNC(CreateStreamHeader), true);
	strlNode->AddChild(BIND_FUNC(CreateAVIStreamHeader), false);
	strlNode->AddChild(BIND_FUNC(CreateBitmapInfoHeader), false);
	Node* moviNode = _rootNode->AddChild(BIND_FUNC(CreateMOVIHeader), true);

	for (unsigned int index = 0; index < _frameCount; ++index)
	{
		moviNode->AddChild(BIND_FUNC(AddFrame), false);
	}
	
	BuildAVI(_dataBuffer, _rootNode);
}

AVIContainer::~AVIContainer()
{

}

void AVIContainer::SetFourCC(uint32_t* fourCC, const char value[4])
{
	if (value == nullptr)
	{
		fourCC[3] = 0;
		fourCC[2] = 0;
		fourCC[1] = 0;
		fourCC[0] = 0;
		//memcpy(fourCC, 0, sizeof(uint32_t));
		return;
	}

	*fourCC = (uint32_t)value[3] << 24 |
		(uint32_t)value[2] << 16 |
		(uint32_t)value[1] << 8 |
		(uint32_t)value[0];
}

void* AVIContainer::GetFileData(unsigned int& offset, unsigned int blockSize)
{
	(void)blockSize;

	offset = _fileSize;
	return _dataBuffer;
}

void AVIContainer::WriteToFile(std::string filePath)
{
	std::ofstream out(filePath, std::ofstream::binary | std::ofstream::out);
	out.write(reinterpret_cast<const char*>(_dataBuffer), _fileSize);
	out.close();
}


unsigned int AVIContainer::CreateRIFFHeader(void* buffer, unsigned int offset)
{
	std::cout << "RIFF" << std::endl;

	AVIList hdr;
	SetFourCC(&hdr.FourCC, "RIFF");
	hdr.Size = sizeof(uint32_t);	// Size of type, FourCC and Size are not taken into account, according to specs
	//hdr.listsize = calculateSize(RIFF);
	SetFourCC(&hdr.Type, "AVI ");
	memcpy((uint8_t*)buffer + offset, &hdr, AVIListSize);

	return AVIListSize;
}


unsigned int AVIContainer::CreateHDRLHeader(void* buffer, unsigned int offset)
{
	std::cout << "hdrl" << std::endl;
	AVIList hdr;
	SetFourCC(&hdr.FourCC, "LIST");
	hdr.Size = 4;
	SetFourCC(&hdr.Type, "hdrl");
	memcpy((uint8_t*)buffer + offset, &hdr, AVIListSize);

	return AVIListSize;
}

unsigned int AVIContainer::CreateAVIMainHeader(void* buffer, unsigned int offset)
{
	std::cout << "avih" << std::endl;
	AVIMainHeader hdr;
	SetFourCC(&hdr.FourCC, "avih");
	hdr.cb = AVIMainHeaderSize - 8;
	hdr.MicroSecPerFrame = 2854;
	hdr.MaxBytesPerSec = 100000;
	hdr.PaddingGranularity = 0;
	hdr.Flags = 0;
	hdr.TotalFrames = _frameCount;
	hdr.InitialFrames = 0;
	hdr.Streams = 1;
	hdr.SuggestedBufferSize = 100000;
	hdr.Width = _width;
	hdr.Height = _height;
	hdr.Reserved[0] = hdr.Reserved[1] = hdr.Reserved[2] = hdr.Reserved[3] = 0;

	memcpy((uint8_t*)buffer + offset, &hdr, AVIMainHeaderSize);

	return AVIMainHeaderSize;
}

unsigned int AVIContainer::CreateAVIStreamHeader(void* buffer, unsigned offset)
{
	std::cout << "strh" << std::endl;
	AVIChunk hdr;
	SetFourCC(&hdr.FourCC, "strh");
	hdr.Size = AVIStreamHeaderSize;
	memcpy((uint8_t*)buffer + offset, &hdr, AVIChunkSize);

	AVIStreamHeader streamHdr;
	SetFourCC(&streamHdr.FourCCType, "vids");
	SetFourCC(&streamHdr.fccHandler, "RAW ");
	streamHdr.Flags = 0;
	streamHdr.Priority = 0;
	streamHdr.Language = 0;
	streamHdr.InitialFrames = 0;
	streamHdr.Scale = 1;
	streamHdr.Rate = 30;
	streamHdr.Start = 0;
	streamHdr.Length = streamHdr.Rate / streamHdr.Scale;
	streamHdr.SuggestedBufferSize = 0;
	streamHdr.Quality = -1;
	streamHdr.SampleSize = 0;
	streamHdr.rcFrame.left = 0;
	streamHdr.rcFrame.right = _width;
	streamHdr.rcFrame.top = 0;
	streamHdr.rcFrame.bottom = _height;

	memcpy((uint8_t*)buffer + offset + AVIChunkSize, &streamHdr, AVIStreamHeaderSize);

	return AVIStreamHeaderSize + AVIChunkSize;
}

unsigned int AVIContainer::CreateBitmapInfoHeader(void* buffer, unsigned offset)
{
	std::cout << "strf" << std::endl;
	AVIChunk hdr;
	SetFourCC(&hdr.FourCC, "strf");
	hdr.Size = BitmapInfoHeaderSize;
	memcpy((uint8_t*)buffer + offset, &hdr, AVIChunkSize);


	BitmapInfoHeader bitmapHdr;
	bitmapHdr.Size = BitmapInfoHeaderSize;
	bitmapHdr.Width = _width;
	bitmapHdr.Height = _height;
	bitmapHdr.Planes = 1;
	bitmapHdr.BitCount = 24;
	SetFourCC(&bitmapHdr.Compression, nullptr);
	bitmapHdr.SizeImage = 0;
	bitmapHdr.XPelsPerMeter = 2835;
	bitmapHdr.YPelsPerMeter = bitmapHdr.XPelsPerMeter;
	bitmapHdr.ClrUsed = 0;
	bitmapHdr.ClrImportant = 0;

	memcpy((uint8_t*)buffer + offset + AVIChunkSize, &bitmapHdr, BitmapInfoHeaderSize);

	return BitmapInfoHeaderSize + AVIChunkSize;
}

int GenerateRGBStripes(uint8_t* frame, unsigned int width, unsigned int height)
{
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			frame[3 * (i * height) + 3 * j + 0] = i < (width / 3) ? 255 : 0;
			frame[3 * (i * height) + 3 * j + 1] = i >= (width / 3) && i < (2 * width / 3) ? 255 : 0;
			frame[3 * (i * height) + 3 * j + 2] = i >= (2 * width / 3) ? 255 : 0;
		}
	}
	return 0;
}

unsigned int AVIContainer::AddFrame(void* buffer, unsigned int offset)
{
	AVIChunk frame;
	SetFourCC(&frame.FourCC, "00db");
	frame.Size = _frameSize;
	memcpy((uint8_t*)buffer + offset, &frame, AVIChunkSize);

	uint8_t* frameData = new uint8_t[_frameSize];
	GenerateRGBStripes(frameData, _width, _height);

	memcpy((uint8_t*)buffer + offset + AVIChunkSize, frameData, _frameSize);

	return _frameSize + AVIChunkSize;
}

unsigned int AVIContainer::CreateStreamHeader(void* buffer, unsigned int offset)
{
	std::cout << "strl" << std::endl;
	AVIList hdr;
	SetFourCC(&hdr.FourCC, "LIST");
	hdr.Size = 4;
	SetFourCC(&hdr.Type, "strl");
	memcpy((uint8_t*)buffer + offset, &hdr, AVIListSize);

	return AVIListSize;
}

unsigned int AVIContainer::CreateMOVIHeader(void* buffer, unsigned int offset)
{
	std::cout << "movi" << std::endl;
	AVIList hdr;
	SetFourCC(&hdr.FourCC, "LIST");
	hdr.Size = 4;
	SetFourCC(&hdr.Type, "movi");
	memcpy((uint8_t*)buffer + offset, &hdr, AVIListSize);

	return AVIListSize;
}

bool AVIContainer::BuildAVI(uint8_t* dataBuffer, Node* node)
{
	unsigned int offset = 0;

	_fileSize = node->Execute(dataBuffer, offset, node->GetRequiresSize());

	return true;
}
