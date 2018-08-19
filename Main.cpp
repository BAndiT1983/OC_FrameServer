#include <iostream>
#include <functional>
#include <vector>

#include "FUSEHandler.h"

#include "DataProvider/AVIContainer.h"
#include "DataProvider/SimpleTree.h"


int main(int argc, char* argv[])
{
	std::cout << "OC_FrameServer" << std::endl;
	std::cout << "-------" << std::endl;

	//std::cout << "Header size: " << sizeof(AVIList) << std::endl;

	auto aviContainer = new AVIContainer(320, 240, 30, 90);
	aviContainer->WriteToFile("test.avi");
	delete aviContainer;

	//uint8_t* dataBuffer = new uint8_t[1024];
	//Node* mainNode = new Node(Test1);

	//BuildAVI(dataBuffer, mainNode);


	//AVIList testList;
	//AviContainer container;
	//container.SetFourCC(&testList.FourCC, "TST1");
	//testList.Size = 1234;
	//container.SetFourCC(&testList.Type, "AVI ");

	//fuse_operations fuse_ops = {};
	//fuse_ops.getattr = getattr_callback;
	//fuse_ops.open = open_callback;
	//fuse_ops.read = read_callback;
	//fuse_ops.readdir = readdir_callback;

	//return fuse_main(argc, argv, &fuse_ops, NULL);

	return 0;
}
