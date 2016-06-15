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
        using BitArray = std::array<uint8_t, ((capacity + 7) & ~7) / 8>;
    public:
        BitTree();
        BitTree(BitArray bits);

        BitTreeNode<capacity> root();
        BitTreeNode<capacity> node(size_t index);

        const BitArray &bits() const;
        BitArray &bits();

    private:
        BitArray bits_;
    };

    static uint8_t serializeBitTree(BitTree<8> tree);
    static uint16_t serializeBitTree(BitTree<16> tree);
    static uint32_t serializeBitTree(BitTree<32> tree);
    static uint64_t serializeBitTree(BitTree<64> tree);

    static BitTree<8> deserializeBitTree(uint8_t value);
    static BitTree<16> deserializeBitTree(uint16_t value);
    static BitTree<32> deserializeBitTree(uint32_t value);
    static BitTree<64> deserializeBitTree(uint64_t value);

}

#endif // SNW_BIT_TREE_H
