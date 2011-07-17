#include "radixTree.hpp"

namespace kick_ass {

int rNode::nodeCnt = 0;

int numFromLetter (char ch)
{
    if (ch >= 97 && ch <= 122)
	ch -= 32;
    return ch - 65 + 1;
}

inline int hashCode(int num, char ch)
{
    return num*100 + numFromLetter(ch);
}

int findPatMatch(const std::string &str1, const std::string &str2)
{
    int i = 0;
    while (i < str1.size()) {
	if (str1[i] == str2[i])
	    ++i;
	else
	    break;
    }
    return i;
}

void rNode::insert (std::string &word, _transMap &transMap)
{
    /*find if current node has a transition on the 1st char of string to be
    inserted*/
    int idx = hashCode(_id, word[0]);
    if (transMap.find(idx) == transMap.end()) {
	rNode *newNode = new rNode(false, true);
	rEdge *edge = new rEdge(this, newNode, word);
	newNode->parentEdge(edge);
	transMap[idx] = edge;
	return;
    }
    /*find pattern match b/w e and word*/
    rEdge &e = *(transMap[idx]);
    int match = findPatMatch(e.value(), word);
    if (match == word.size() && match == e.value().size()) {
	e.to().leaf(true);
	return;
    }
    if (match == e.value().size()) {
	std::string _tmp = word.substr(match, word.size());
	e.to().insert(_tmp, transMap);
    }
    else {
	/*split existing edge*/
	/*create a new internal node*/
	rNode *intNode = new rNode(false, false);
	/*create a new edge*/
	std::string _tmp = e.value().substr(0, match);
	rEdge *splitEdge = new rEdge(this, intNode, _tmp);
	intNode->parentEdge(splitEdge);
	/*erase e from parent's map, and insert the splitEdge into it*/
	transMap[hashCode(_id, splitEdge->value()[0])] = splitEdge;

	/*modify curr edge*/
	e.value().erase(0, match);
	e.from(intNode);
	transMap[hashCode(intNode->id(), e.value()[0])] = &e;

	if (match == word.size())
	    splitEdge->to().leaf(true);
	else {
	    /*create node and edge for the new leaf*/
	    rNode *leaf = new rNode(false, true);
	    std::string _tmp = word.substr(match, word.size());
	    rEdge *edge2Leaf = new rEdge(intNode, leaf, _tmp);
	    leaf->parentEdge(edge2Leaf);
	    transMap[hashCode(intNode->id(), edge2Leaf->value()[0])] = edge2Leaf;
	}
    }
    return;
}

/*tells if a prefix is present in corpus?*/
rNode* rNode::lookup (std::string &qury, _transMap &transMap)
{
    int idx = hashCode(_id, qury[0]);
    if (transMap.find(idx) == transMap.end())
	return NULL;
    rEdge &leavEdge = *(transMap[idx]);
    int match = findPatMatch(leavEdge.value(), qury);
    if (match < 0)
	return NULL;
    if (match == qury.size())
	return &(leavEdge.to());
    std::string _tmp = qury.substr(match, qury.size());
    return leavEdge.to().lookup(_tmp, transMap);
}

void rNode::edgesFrom (std::vector<rEdge*> &edges, _transMap &transMap)
{
    for (int i = 1; i <= 26; i++) {
	int idx = _id*100 + i;
	if (transMap.find(idx) == transMap.end())
	    continue;
	edges.push_back(transMap[idx]);
    }
    return;
}

int rNode::remove (std::string &qury, _transMap &transMap)
{
    rNode *pres = NULL;
    if (!(pres = lookup(qury, transMap)))
	return -1; /*string not present in tree*/
    if (!(pres->isLeaf)) {
	pres->leaf(false);
	return 0; /*string is present at internal node => done!, no deletion of nodes to be done*/
    }

    rEdge &inEdge = pres->inComingEdge();
    rNode &parent = inEdge.from();
    /*erase the edge from parent's map*/
    transMap.erase(hashCode(parent.id(), inEdge.value().at(0)));//erase will call ~rEdge, which in turn will call ~rNode for pres
    /*now delete the edge and node*/
    /*if parent has only one child, then merge parent with the child*/
    std::vector<rEdge*> leavingEdges;
    parent.edgesFrom(leavingEdges, transMap);
    if (leavingEdges.size() == 1) {
	rEdge &parInEdge = parent.inComingEdge();
	rNode &granPar = parInEdge.from();
	std::string tmp = parInEdge.value() + leavingEdges[0]->value();
	rEdge &onlyEdge = *(leavingEdges[0]);
	onlyEdge.value().assign(tmp, 0, tmp.size());
	onlyEdge.from(granPar);
	/*now erase parInEdge from granPar's map*/
	transMap.erase(hashCode(granPar.id(), parInEdge.value().at(0)));
	/*insert onlyEdge into granPar's map*/
	transMap[hashCode(granPar.id(), onlyEdge.value().at(0))] = &onlyEdge;
    }
    return 0;
}

void rNode::getLeaves(std::string &qury, _transMap &transMap, std::vector<std::string> &retVal)
{
    if (isLeaf)
	retVal.push_back(qury);
    std::vector<rEdge*> leavingEdges;
    edgesFrom(leavingEdges, transMap);
    if (!leavingEdges.size())
	return;
    for (int i = 0; i < leavingEdges.size(); i++) {
	std::string _tmp = qury + leavingEdges[i]->value();
	leavingEdges[i]->to().getLeaves(_tmp, transMap, retVal);
    }
    return;
}

void rNode::wordsStartingWith(std::string &qury, std::string &orig, _transMap &transMap, std::vector<std::string> &result)
{
    int idx = hashCode(_id, qury[0]);
    if (transMap.find(idx) == transMap.end())
	return;
    rEdge &leavEdge = *(transMap[idx]);
    int match = findPatMatch(leavEdge.value(), qury);
    if (match == qury.size() && match == leavEdge.value().size())
	leavEdge.to().getLeaves(orig, transMap, result);
    else if (match == leavEdge.value().size()) {
	std::string _tmp = qury.substr(match, qury.size());
	leavEdge.to().wordsStartingWith(_tmp, orig, transMap, result);
    }
    else if (match < leavEdge.value().size()) {
	std::string newQry = orig + leavEdge.value().substr(match, leavEdge.value().size());
	leavEdge.to().getLeaves(newQry, transMap, result);
    }
    return;
}

std::vector<std::string>& radixTree::wordsStartingWith(std::string &query)
{
    std::string copy = query;
    std::vector<std::string> *result = new std::vector<std::string>();
    result->clear();
    root->wordsStartingWith(copy, query, *transitionMap, *result);
    return *result;
}

radixTree::~radixTree()
{
    for (_transMap::iterator itr = transitionMap->begin(); itr !=
	    transitionMap->end(); ++itr) {
	rEdge *edge = itr->second;
	delete edge;
    }
    transitionMap = NULL;
    delete root;
    root = NULL;
}

}
