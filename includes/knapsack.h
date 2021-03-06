// Knapsack class
// Version f08.1
class knapsack
{
   public:
      knapsack();
      knapsack(ifstream &fin);
      knapsack(const knapsack &);
      int getCost(int) const;
      int getValue(int) const;
      int getCost() const;
      int getValue() const;
      int getNumObjects() const;
      int getCostLimit() const;
      void printSolution();
      void select(int);
      void unSelect(int);
      bool isSelected(int) const;
      void selectList(vector<int> list);
      void selectList(vector<bool> list); 
      void unSelectAll(void);
      int getMaxRatioIndex(void);
      int getMaxRatioUnselectedIndex(void);
      void setRatioOff(int index);
      void resetValueCostRatio(void);
      bool allItemsOff(void);
      float bound(int layer);
      vector<int> getSelectedItems(void);

   private:
      int numObjects;
      int costLimit;
      vector<int> value;
      vector<int> cost;
      vector<float> valueCostRatio;
      vector<bool> selected;
      int totalValue;
      int totalCost;
};

knapsack::knapsack() {

}

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.
{
   int n, b, j, v, c;
   
   fin >> n;  // read the number of objects
   fin >> b;  // read the cost limit

   numObjects = n;
   costLimit = b;
   
   value.resize(n);
   cost.resize(n);
   selected.resize(n);
   valueCostRatio.resize(n);
   
   for (int i = 0; i < n; i++)
   {
      fin >> j >> v >> c;
      value[j] = v;
      cost[j] = c;
      valueCostRatio[j] = (1.0 * v) / c;
      unSelect(j);
   }
   totalValue = 0;
   totalCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor.
{
   int n = k.getNumObjects();
   
   value.resize(n);
   cost.resize(n);
   selected.resize(n);
   numObjects = k.getNumObjects();
   costLimit = k.getCostLimit();

   totalCost = 0;
   totalValue = 0;

   for (int i = 0; i < n; i++)
   {
      value[i] = k.getValue(i);
      cost[i] = k.getCost(i);
      if (k.isSelected(i))
	 select(i);
      else
	 unSelect(i);
   }
}

int knapsack::getNumObjects() const
{
   return numObjects;
}

int knapsack::getCostLimit() const
{
   return costLimit;
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");
   
   return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getCost");

   return cost[i];
}

int knapsack::getCost() const
// Return the cost of the selected objects.
{
   return totalCost;
}

int knapsack::getValue() const
// Return the value of the selected objects.
{
   return totalValue;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
   cout << "------------------------------------------------" << endl;
   cout << "Num objects: " << k.getNumObjects() << " Cost Limit: " << k.getCostLimit() << endl;

   int totalValue = 0;
   int totalCost = 0;
   
   for (int i = 0; i < k.getNumObjects(); i++)
   {
      totalValue += k.getValue(i);
      totalCost += k.getCost(i);
   }

   cout << "Total value: " << totalValue << endl;
   cout << "Total cost: " << totalCost << endl << endl;

   for (int i = 0; i < k.getNumObjects(); i++)
      cout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   cout << endl;

   return ostr;
}

void knapsack::printSolution()
// Prints out the solution.
{
   cout << "------------------------------------------------" << endl;

   cout << "Total value: " << getValue() << endl;
   cout << "Total cost: " << getCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < getNumObjects(); i++)
      if (isSelected(i))
	 cout << i << "  " << getValue(i) << " " << getCost(i) << endl;

   cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
   for (unsigned int i = 0; i < v.size(); i++)
      cout << v[i] << endl;

   return ostr;
}

void knapsack::select(int i)
// Select object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::Select");

   if (selected[i] == false)
   {
      selected[i] = true;
      totalCost = totalCost + getCost(i);
      totalValue = totalValue + getValue(i);
      // setRatioOff(i); //UNCOMMENT IF RUNNING PROJECT 1
   }
}

void knapsack::unSelect(int i)
// unSelect object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::unSelect");

   if (selected[i] == true)
   {
      selected[i] = false;
      totalCost = totalCost - getCost(i);
      totalValue = totalValue - getValue(i);
   }
}

bool knapsack::isSelected(int i) const
// Return true if object i is currently selected, and false otherwise.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");

   return selected[i];
}

void knapsack::selectList(vector<int> list) {
   for (unsigned int i = 0; i < list.size(); i++) {
      select(list[i]);
   } 
}

void knapsack::selectList(vector<bool> list) {
   for (unsigned int i = 0; i < list.size(); i++) {
      if (list[i]) {
         select(i);
      }
      else {
         unSelect(i);
      }
   } 
}

void knapsack::unSelectAll(void) {
   for (int i = 0; i < getNumObjects(); i++) {
      if (isSelected(i)) {
         unSelect(i);
      }
   }
}

// returns index of highest ratio item in list
int knapsack::getMaxRatioIndex(void) {
   int max = 0;
   for (int i = 1; i < getNumObjects(); i++) {
      if (valueCostRatio[i] > valueCostRatio[max]) {
         max = i;
      }
   }
   return max;
}

int knapsack::getMaxRatioUnselectedIndex(void) {
   int max_index = -1;
   float max_ratio = 0.0;
   for (int i = 0; i < getNumObjects(); i++) {
      if (valueCostRatio[i] > max_ratio && isSelected(i) == false) {
         max_index = i;
         max_ratio = valueCostRatio[i];
      }
   }
   return max_index;
}

// sets index to -1.0 in valueCostRatio
void knapsack::setRatioOff(int index) {
   if (index < getNumObjects()){
      valueCostRatio[index] = -1.0;
   }
}

// checks if all values in valueCostRatio are negative
// if so, all items are off so return true
bool knapsack::allItemsOff(void) {
   for (int i = 0; i < getNumObjects(); i++) {
      if (valueCostRatio[i] >= 0) {
         return false;
      }
   }
   return true;
}

void knapsack::resetValueCostRatio(void) {
   for (int i = 0; i < getNumObjects(); i++) {
      valueCostRatio[i] = 1.0 * value[i] / cost[i];
   }
}

// returns upper bound on value
float knapsack::bound(int layer) {
   float upper_bound = 0;
   int index = 0;

   // to calculate a tighter upper bound, we should only be able to select items that are beyond the current layer
   for (int i = 0; i <= layer; i++) {
      setRatioOff(i);
   }

   while ((index = getMaxRatioUnselectedIndex()) >= 0) {
      if ((getCost() + getCost(index)) > getCostLimit()) {
         break;
      } else {
         select(index);
      }
   }

   upper_bound = 1.0 * getValue();
   // if all the items available are selected, then the upper bound is the value of whatever is already in the knapsack
   if (getMaxRatioUnselectedIndex() >= 0) {
      upper_bound += 1.0 * ((getCostLimit() - getCost()) * valueCostRatio[getMaxRatioUnselectedIndex()]);
   }
   
   // then make sure we reset the value cost ratio so as not to mess up future calculations
   resetValueCostRatio();
   return upper_bound;
}

// returns a list of the selected indices
vector<int> knapsack::getSelectedItems(void) {
   vector<int> selectedItems;

   for (int i = 0; i < getNumObjects(); i++) {
      if (selected[i]) {
         selectedItems.push_back(i);
      }
   }

   return selectedItems;
}