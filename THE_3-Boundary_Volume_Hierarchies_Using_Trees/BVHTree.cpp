#include "BVHTree.h"

BVHTree::BVHTree() {
	root = NULL;
}
BVHTree::~BVHTree() {
	DestroyRecursively(root);
}
void BVHTree::DestroyRecursively(BVHTreeNode* node) {
	if (node!=0)
	{
		DestroyRecursively(node->leftChild);
		DestroyRecursively(node->rightChild);
		map.erase(node->name);
		delete node;
		node = NULL;
	}

}

void BVHTree::addBVHMember(AABB objectArea, std::string name) {
	bool yes = true;
	if (root == NULL) {
		BVHTreeNode *newNode = new BVHTreeNode(objectArea, name, yes);
		map[name] = newNode;
		root = newNode;
	}
	else if (root->isLeaf){
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, yes);
		BVHTreeNode* newRoot = new BVHTreeNode(objectArea+root->aabb, "branch", false);
		BVHTreeNode* temp = root;
		root = newRoot;
		temp->parent = root;
		newNode->parent = root;
		newRoot->leftChild = newNode;
		newRoot->rightChild = temp;
		map[name] = newNode;
	}
	else {
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, yes);
		BVHTreeNode* branchNode = root;
		bool where;
		while (branchNode->isLeaf == false) {
			int increaseInRightTreeSize = AABB::unionArea(newNode->aabb, branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb, branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();
			if (increaseInRightTreeSize < increaseInLeftTreeSize) {
				branchNode = branchNode->rightChild;
				where = false;
			}
			else {
				branchNode = branchNode->leftChild;
				where = true;
			}
		}
		BVHTreeNode* newBranch = new BVHTreeNode(objectArea + branchNode->aabb, "branch", false);
		newBranch->leftChild = newNode;
		newBranch->rightChild = branchNode;
		newBranch->parent = branchNode->parent;
		branchNode->parent = newBranch;
		newNode->parent = newBranch;
		if (where) {
			newBranch->parent->leftChild=newBranch;
		}
		else {
			newBranch->parent->rightChild = newBranch;
		}
		map[name] = newNode;
		BVHTreeNode* dummy = newNode->parent;
		while (dummy!= 0){
			dummy->aabb = dummy->leftChild->aabb + dummy->rightChild->aabb;
			dummy = dummy->parent;
		}
	}

}

void BVHTree::removeBVHMember(std::string name) {
	if (map.find(name) != map.end()) {
		BVHTreeNode* toRemove = map[name];
		BVHTreeNode* dummy = toRemove->parent;
		BVHTreeNode* sibling;
		if (dummy == 0) {
			delete toRemove;
			root = NULL;
		}
		else {
			if (toRemove->parent->leftChild == toRemove) {
				sibling = toRemove->parent->rightChild;		
			}
			else {
				sibling = toRemove->parent->leftChild;
			}
			if (dummy->parent->leftChild == dummy) {
				sibling->parent = dummy->parent;
				dummy->parent->leftChild = sibling;

			}
			else {
				sibling->parent = dummy->parent;
				dummy->parent->rightChild = sibling;
			}	
			map.erase(name);
			delete dummy;
			delete toRemove;
			dummy = sibling->parent;
			while (dummy != 0) {
				dummy->aabb = dummy->leftChild->aabb + dummy->rightChild->aabb;
				dummy = dummy->parent;
			}
		}
	}
}




void BVHTree::moveBVHMember(std::string name, AABB newLocation) {
	if (map.find(name) != map.end()) {
		BVHTreeNode* toMove = map[name];
		if (toMove == root) {
			root->aabb = newLocation;
		}
		else {
			if (AABB::unionArea(toMove->parent->aabb, newLocation) == AABB::unionArea(toMove->parent->aabb, toMove->aabb)) {
				toMove->aabb = newLocation;
			}
			else {

				removeBVHMember(toMove->name);
				addBVHMember(newLocation, name);
			}
		}
	}
}





std::vector<std::string> BVHTree::getCollidingObjects(AABB object){
	std::vector<std::string> collidevector;
	recursivecollide(root, collidevector, object);
	return collidevector;
}


void BVHTree::recursivecollide(BVHTreeNode* node, std::vector<std::string>& vec, AABB object){
	if (node != 0) {
		recursivecollide(node->leftChild, vec, object);
		recursivecollide(node->rightChild, vec, object);
		if (object.collide(node->aabb)&& node->isLeaf) {
			vec.push_back(node->name);
		}
	}

}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}