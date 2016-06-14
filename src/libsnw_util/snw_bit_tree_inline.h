#ifndef SNW_BIT_TREE_INLINE_H
#define SNW_BIT_TREE_INLINE_H

template<size_t capacity>
Snowda::BitTreeNode<capacity>::BitTreeNode()
    : tree_(nullptr)
    , index_(0)
{
}

template<size_t capacity>
Snowda::BitTreeNode<capacity>::BitTreeNode(BitTree<capacity> *tree, size_t index)
    : tree_(tree)
    , index_(index)
{
    assert(tree_ != nullptr);
    assert(index_ < capacity);
}

template<size_t capacity>
size_t Snowda::BitTreeNode<capacity>::index() const
{
    assert(tree_ != nullptr);

    return index_;
}

template<size_t capacity>
Snowda::BitTreeNode<capacity> Snowda::BitTreeNode<capacity>::parent() const
{
    assert(tree_ != nullptr);

    if (index_ > 0) {
        return BitTreeNode(tree_, (index_ - 1) >> 1);
    }
    else {
        return BitTreeNode();
    }
}

template<size_t capacity>
Snowda::BitTreeNode<capacity> Snowda::BitTreeNode<capacity>::leftChild() const
{
    assert(tree_ != nullptr);

    const size_t childIndex = (index_ << 1) + 1;
    if (childIndex < capacity) {
        return BitTreeNode(tree_, childIndex);
    }
    else {
        return BitTreeNode();
    }
}

template<size_t capacity>
Snowda::BitTreeNode<capacity> Snowda::BitTreeNode<capacity>::rightChild() const
{
    assert(tree_ != nullptr);

    const size_t childIndex = (index_ << 1) + 2;
    if (childIndex < capacity) {
        return BitTreeNode(tree_, childIndex);
    }
    else {
        return BitTreeNode();
    }
}

template<size_t capacity>
bool Snowda::BitTreeNode<capacity>::value() const
{
    size_t byteIndex;
    size_t bitIndex;
    std::tie(byteIndex, bitIndex) = valuePosition();

    return (tree_->bits_[byteIndex] & (1 << bitIndex)) != 0;
}

template<size_t capacity>
void Snowda::BitTreeNode<capacity>::setValue()
{
    size_t byteIndex;
    size_t bitIndex;
    std::tie(byteIndex, bitIndex) = valuePosition();

    tree_->bits_[byteIndex] |= (1 << bitIndex);
}

template<size_t capacity>
void Snowda::BitTreeNode<capacity>::clearValue()
{
    size_t byteIndex;
    size_t bitIndex;
    std::tie(byteIndex, bitIndex) = valuePosition();

    tree_->bits_[byteIndex] &= ~(1 << bitIndex);
}

template<size_t capacity>
Snowda::BitTreeNode<capacity>::operator bool() const
{
    return tree_ != nullptr;
}

template<size_t capacity>
std::tuple<size_t, size_t> Snowda::BitTreeNode<capacity>::valuePosition() const
{
    assert(tree_ != nullptr);

    return std::make_tuple(index_ >> 3, index_ & 7);
}

template<size_t capacity>
Snowda::BitTree<capacity>::BitTree()
{
    memset(bits_.data(), 0, bits_.size());
}

template<size_t capacity>
Snowda::BitTreeNode<capacity> Snowda::BitTree<capacity>::root()
{
    return node(0);
}

template<size_t capacity>
Snowda::BitTreeNode<capacity> Snowda::BitTree<capacity>::node(size_t index)
{
    assert(index < capacity);

    return BitTreeNode<capacity>(this, index);
}

#endif // SNW_BIT_TREE_INLINE_H
