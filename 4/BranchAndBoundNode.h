class BranchAndBoundNode {
public:
    BranchAndBoundNode(vector<int>, int, knapsack &);
    BranchAndBoundNode(const BranchAndBoundNode &);
    BranchAndBoundNode(void);
    bool getFathomed(void) const;
    void setFathomed(bool f);
    vector<int> getSelectedItems(void) const;
    int getLayer(void) const;
    float getUpperBound(void) const;
    int getSelectedItemsValue(void) const;
    void printItems(void) const;
private:
    bool fathomed;
    vector<int> selected_items;
    int layer;
    float upper_bound;
    int selected_items_value;
};

BranchAndBoundNode::BranchAndBoundNode(vector<int> items, int current_layer, knapsack &k) {
    selected_items = items;
    layer = current_layer;

    k.unSelectAll();
    k.selectList(selected_items);
    upper_bound = k.bound();
    selected_items_value = k.getValue();

    cout << "New BnB node: Value = " << selected_items_value << " upper_bound = " << upper_bound << endl;
    printItems();
}

BranchAndBoundNode::BranchAndBoundNode(const BranchAndBoundNode &other) {
    fathomed = other.getFathomed();
    selected_items = other.getSelectedItems();
    layer = other.getLayer();
    upper_bound = other.getUpperBound();
    selected_items_value = other.getSelectedItemsValue();
}

BranchAndBoundNode::BranchAndBoundNode(void) {

}

bool BranchAndBoundNode::getFathomed(void) const {
    return fathomed;
}

void BranchAndBoundNode::setFathomed(bool f) {
    fathomed = f;
}

vector<int> BranchAndBoundNode::getSelectedItems(void) const {
    vector<int> copy = selected_items;
    return copy;
}

int BranchAndBoundNode::getLayer(void) const {
    return layer;
}

float BranchAndBoundNode::getUpperBound(void) const {
    return upper_bound;
}

int BranchAndBoundNode::getSelectedItemsValue(void) const {
    return selected_items_value;
}

void BranchAndBoundNode::printItems(void) const {
    cout << "Selected Items: ";
    for (int i = 0; i < selected_items.size(); i++) {
        cout << selected_items[i] << " ,";
    }
    cout << endl;
}