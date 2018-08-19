#ifndef SIMPLETREE_H
#define SIMPLETREE_H

#include <vector>
#include <functional>
#include <cstring>
#include <iostream>

typedef std::function<unsigned int(void*, unsigned int)> AVIFunc;

// void* - pointer to buffer, unsigned int - offset, returns unsigned int - size of generated data

//unsigned int Test1(void* dataBuffer, unsigned int offset)
//{
//	unsigned int size = 3;
//	memcpy(dataBuffer, "123", size);
//	return size;
//}

class Node
{
	std::vector<Node*> _nodeList;

	AVIFunc _func;
	bool _requiresSize;

public:
	Node(AVIFunc func, bool requiresSize) :
		_nodeList({}),
		_func(func),
		_requiresSize(requiresSize)
	{
	}

	Node* AddChild(AVIFunc func, bool requiresSize)
	{
		Node* node = new Node(func, requiresSize);
		_nodeList.push_back(node);

		return node;
	}

	unsigned int Execute(void* dataBuffer, unsigned int offset, bool calculateSize)	// calculateSize -> gathers size of nested chunks and writes it to Size attribute of AVIList or AVICHunk
	{
		unsigned int size = _func(dataBuffer, offset);
		unsigned int childSize = 0;

		for (Node* child : _nodeList)
		{
			childSize += child->Execute(dataBuffer, offset + size + childSize, calculateSize);
		}

		if (calculateSize)
		{
			//childSize += sizeof(uint32_t);
			uint32_t headerSize = 0;// = > (dataBuffer[0]) + offset + sizeof(uint32_t);
			memcpy(&headerSize, dataBuffer + offset + sizeof(uint32_t), sizeof(uint32_t));
			headerSize += childSize;
			memcpy(dataBuffer + offset + sizeof(uint32_t), &headerSize, sizeof(uint32_t));
			std::cout << "Written bytes: " << headerSize << std::endl;
		}

		return size + childSize;
	}

	bool GetRequiresSize() const
	{
		return _requiresSize;
	}
};


#endif // SIMPLETREE_H
