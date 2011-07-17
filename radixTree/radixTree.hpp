#ifndef _RADIX_TREE_H
#define _RADIX_TREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace kick_ass {

class rEdge;/*forward declaration*/
typedef std::map<int, rEdge*> _transMap;
class rNode {
    public:
	rNode() : isRoot(false), isLeaf(false) {
	    _id = ++rNode::nodeCnt;
	}
	rNode(bool root = false, bool leaf = false) {
	    _id = ++(rNode::nodeCnt);
	    isRoot = root;
	    isLeaf = leaf;
	    _parentEdge = NULL;
	}
	~rNode() {
	    _parentEdge = NULL;
	}
	rEdge& inComingEdge() {
	    return *_parentEdge;
	}
	void parentEdge(rEdge *e) {
	    _parentEdge = e;
	    return;
	}
	void leaf(bool x) { isLeaf = x; }
	bool leaf() { return isLeaf; }
	bool root() { return isRoot; }
	int id() { return _id; }
	void insert (std::string &word, _transMap&);
	void wordsStartingWith(std::string &query, std::string &original, _transMap&, std::vector<std::string>&);
	void getLeaves(std::string&, _transMap&, std::vector<std::string>&);
	void edgesFrom(std::vector<rEdge*>&, _transMap&);
	int remove(std::string &str, _transMap&);
	rNode* lookup(std::string&, _transMap&);
    private:
	rNode(const rNode&);
	const rNode& operator=(const rNode&);
	
	static int nodeCnt;
	int _id;
	bool isRoot;
	bool isLeaf;
	rEdge *_parentEdge;
};

class rEdge {
    public:
	rEdge(rNode *fNode, rNode *tNode, std::string &str) {
	    fromNode = fNode;
	    toNode = tNode;
	    _value = new std::string(str);
	}
	rEdge() : fromNode(NULL), toNode(NULL), _value(NULL) {}
	~rEdge() {
	    delete toNode;
	    toNode = NULL;
	    delete _value;
	    _value = NULL;
	}
	std::string& value() { return *_value; }
	rNode& from() { return *fromNode; }
	rNode& to() { return *toNode; }
	void from(rNode &frm) { fromNode = &frm; return; }
	void from(rNode *frm) { fromNode = frm; return; }
	void to(rNode &t) { toNode = &t; return; }
	void to(rNode *t) { toNode = t; return; }
    private:
	rEdge(const rEdge&);
	rEdge& operator=(const rEdge&);

	rNode *fromNode;
	rNode *toNode;
	std::string *_value;
};

class radixTree {
    public:
	radixTree() {
	    root = new rNode(true, false);
	    transitionMap = new _transMap();
	}
	~radixTree();

	void insert(std::string &str) {
	    root->insert(str, *transitionMap);
	    return;
	}
	int remove (std::string &str) {
	    return root->remove(str, *transitionMap);
	}
	bool lookup (std::string &str) {
	    return (root->lookup(str, *transitionMap)) ? true : false;
	}
	std::vector<std::string>& wordsStartingWith(std::string &query);
    private:
	radixTree(const radixTree&);
	radixTree& operator=(const radixTree&);
	
	rNode *root;
	_transMap *transitionMap;
};

}
#endif
