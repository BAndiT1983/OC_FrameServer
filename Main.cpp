#include <iostream>
#include <functional>
#include <vector>

#include "FUSEHandler.h"

#include "DataProvider/AVIContainer.h"
#include <memory>

int main(int argc, char* argv[])
{
	std::cout << "OC_FrameServer" << std::endl;
	std::cout << "-------" << std::endl;

	/*auto aviContainer = new AVIContainer(320, 240, 30, 90);
	aviContainer->WriteToFile("test.avi");
	delete aviContainer;*/
	
	
	//std::unique_ptr<FUSEHandler>* fuseHandler = std::unique_ptr<FUSEHandler>(new FUSEHandler);
	//typedef return_type(ExampleClass::*TypeName)(paramType1, paramTypeN);
	
	const std::unique_ptr<AVIContainer> aviContainer = std::unique_ptr<AVIContainer>(new AVIContainer(320, 240, 30, 90));
	aviContainer->WriteToFile("test2.avi");
	SetAVIContainer(aviContainer.get());

	fuse_operations fuse_ops = {};
	fuse_ops.getattr = getattr_callback;
	fuse_ops.open = open_callback;
	fuse_ops.read = read_callback;
	fuse_ops.readdir = readdir_callback;

	return fuse_main(argc, argv, &fuse_ops, NULL);

	//return 0;
}
