#ifndef SUBSET_CLASSES
#define SUBSET_CLASSES

#include "d_except.h"

// new class to define error specific to Subset class
class NoSuchElementError: public baseException
{
public:
	NoSuchElementError(const string& msg = ""):
		baseException(msg)
	{}
};

// class that generates subsets one at a time
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

// the subset class generates subsets of an array of ints from 0 to m
Subset::Subset(int m) {
	max = m;
}

// checks if there are any remaining subsets
bool Subset::hasNext(void) {
	if (items.size() == 1 && items[0] == max - 1) {
		return false;
	}
	return true;
}

// generates next subset in sequence
vector <int> Subset::nextSubset(void) {
	if (items.size() == 0) {
		// if items is empty, only push 0 onto subset and return
		items.push_back(0);
		return items;
	}
	if (!hasNext()) {
		throw NoSuchElementError("No remaining subsets");
	}

	if (items.back() == max - 1) {
		// if the final item in items is max, we pop it off
		// and increment the new last item to generate a new subset
		items.pop_back();
		items[items.size() - 1] = items.back() + 1;
	} else {
		// if back isn't at max value, we push the next largest value
		// onto items
		items.push_back(items.back() + 1);
	}
	return items;
}

#endif