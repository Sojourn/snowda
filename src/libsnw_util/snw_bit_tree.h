#ifndef SNW_BIT_TREE_H
#define SNW_BIT_TREE_H

namespace Snowda {

    template<size_t capacity>
    class BitTreenode;

    template<size_t capacity>
    class BitTree;

    template<size_t capacity>
    class BitTreeNode {
    public:
        BitTreeNode();
        BitTreeNode(BitTree<capacity> *tree, size_t index);

        size_t index() const;
        BitTreeNode parent() const;
        BitTreeNode leftChild() const;
        BitTreeNode rightChild() const;

        bool value() const;
        void setValue();
        void clearValue();

        explicit operator bool() const;

    private:
        std::tuple<size_t, size_t> valuePosition() const;

    private:
        BitTree<capacity> *tree_;
        size_t index_;
    };

    template<size_t capacity>
    class BitTree {
        static_assert(capacity > 0, "capacity must be non-zero");
        friend class BitTreeNode<capacity>;
    public:
        BitTree();

        BitTreeNode<capacity> root();
        BitTreeNode<capacity> node(size_t index);

    private:
        std::array<uint8_t, ((capacity + 7) & ~7) / 8> bits_;
    };

}

#endif // SNW_BIT_TREE_H
