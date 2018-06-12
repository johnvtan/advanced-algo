class BranchAndBoundNode {
public:
    bool getFathomed(void);
    void setFathomed(bool f);
    vector<int> getSelectedItems(void);
    int getLayer(void);
private:
    bool fathomed;
    vector<int> selected_items;
    int layer;
}

BranchAndBoundNode::BranchAndBoundNode(vector<int> items, int current_layer) {
    selected_items = items;
    layer = current_layer;
}

bool BranchAndBoundNode::getFathomed(void) {
    return fathomed;
}

void BranchAndBoundNode::setFathomed(bool f) {
    fathomed = f;
}

vector<int> BranchAndBoundNode::getSelectedItems(void) {
    vector<int> copy = selected_items;
    return copy;
}

int BranchAndBoundNode::getLayer(void) {
    return layer;
}
