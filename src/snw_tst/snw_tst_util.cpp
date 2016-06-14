#include "snw_tst.h"

using namespace Snowda;

namespace {
    template<size_t capacity>
    size_t tstBitTreeNode(BitTreeNode<capacity> node)
    {
        assert(!node.value());
        node.setValue();
        assert(node.value());
        node.clearValue();
        assert(!node.value());
        node.setValue();

        auto parent = node.parent();
        auto leftChild = node.leftChild();
        auto rightChild = node.rightChild();

        if (node.index() > 0) {
            assert(parent);
        }

        size_t count = 1;
        if (leftChild) {
            count += tstBitTreeNode(leftChild);
        }
        if (rightChild) {
            count += tstBitTreeNode(rightChild);
        }

        return count;
    }

    template<size_t capacity>
    void tstBitTreeHelper();

    template<size_t capacity>
    void tstBitTreeHelper<capacity>()
    {
        BitTree<capacity> tree;
        assert(tstBitTreeNode<capacity>(tree.root()) == capacity);
        tstBitTreeHelper<capacity - 1>();
    }

    template<>
    void tstBitTreeHelper<0>()
    {
    }
}

void Snowda::tstUtil()
{
    TestModule module("Util");
    tstBitTree();
}

void Snowda::tstBitTree()
{
    TestCase test("BitTree");
    tstBitTreeHelper<128>();
}
