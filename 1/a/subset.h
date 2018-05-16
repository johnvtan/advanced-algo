#ifndef SUBSET_CLASSES
#define SUBSET_CLASSES

#include "d_except.h"
class NoSuchElementError: public baseException
{
public:
	NoSuchElementError(const string& msg = ""):
		baseException(msg)
	{}
};

class Subset 
{
   public:
   	Subset(int m);
   	vector <int> nextSubset(void);
   	bool hasNext();
   private:
    vector <int> items;
    int max;
};

Subset::Subset(int m) {
	max = m;
}

bool Subset::hasNext(void) {
	if (items.size() == 1 && items[0] == max - 1) {
		return false;
	}
	return true;
}

vector <int> Subset::nextSubset(void) {
	if (items.size() == 0) {
		items.push_back(0);
		return items;
	}
	if (!hasNext()) {
		throw NoSuchElementError("No remaining subsets");
	}
	if (items.back() == max - 1) {
		items.pop_back();
		items[items.size() - 1] = items.back() + 1;
	} else {
		items.push_back(items.back() + 1);
	}
	return items;
}

#endif