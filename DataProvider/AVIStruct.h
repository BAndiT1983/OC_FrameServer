#ifndef AVISTRUCT_H
#define AVISTRUCT_H

//typedef struct FOURCC
//{
//	uint8_t byte[4];
//};

struct AVIList
{
	uint32_t FourCC;
	uint32_t Size;
	uint32_t Type;
};

struct AVIChunk
{
	uint32_t FourCC;
	uint32_t Size;
};

struct AVIMainHeader
{
	uint32_t FourCC;
	uint32_t cb;
	uint32_t MicroSecPerFrame;
	uint32_t MaxBytesPerSec;
	uint32_t PaddingGranularity;
	uint32_t Flags;
	uint32_t TotalFrames;
	uint32_t InitialFrames;
	uint32_t Streams;
	uint32_t SuggestedBufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t Reserved[4];
};

struct AVIStreamHeader
{
	uint32_t FourCCType;
	//uint32_t cb;
	//uint32_t fccType;
	uint32_t fccHandler;
	uint32_t Flags;
	uint16_t Priority;
	uint16_t Language;
	uint32_t InitialFrames;
	uint32_t Scale;
	uint32_t Rate;
	uint32_t Start;
	uint32_t Length;
	uint32_t SuggestedBufferSize;
	uint32_t Quality;
	uint32_t SampleSize;

	struct
	{
		uint8_t left;
		uint8_t top;
		uint8_t right;
		uint8_t bottom;
	} rcFrame;
};

struct BitmapInfoHeader
{
	uint32_t Size;
	uint32_t Width;
	uint32_t Height;
	uint16_t Planes;
	uint16_t BitCount;
	uint32_t Compression;
	uint32_t SizeImage;
	uint32_t XPelsPerMeter;
	uint32_t YPelsPerMeter;
	uint32_t ClrUsed;
	uint32_t ClrImportant;
};


//enum sizeType {
//	RIFF,
//	HDRL,
//	STRL,
//	MOVI,
//	VIDFRAME
//};

#endif //AVISTRUCT_H
