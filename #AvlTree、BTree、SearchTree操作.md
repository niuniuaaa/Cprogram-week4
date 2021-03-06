# #AvlTree、BTree、SearchTree操作

## ##算法思路

AvlTree：

（1）如果当前的插入操作破坏了平衡性，那么要找出最小不平衡子树，对其执行某种「旋转」操作来调整其中节点的连接关系，使之重新平衡。

（2）如何进行这种「旋转」调整：

根据插入操作发生位置的不同，可以将需要进行调平的情况分成下面四类（记最小不平衡子树的根节点为 N ）：

    情况①：对 N  的左儿子的左子树进行插入之后（左 - 左插入）
    情况②：对 N  的左儿子的右子树进行插入之后（左 - 右插入）
    情况③：对 N  的右儿子的左子树进行插入之后（右 - 左插入）
    情况④：对 N  的右儿子的右子树进行插入之后（右 - 右插入）
（3）单旋转和双旋转

单旋转：可以用于处理情形①和情形④，其中情形①需要右旋（顺时针旋转）调平，情形④需要左旋（逆时针旋转）调平。

双旋转：单次旋转已经可以处理一部分的不平衡，但是针对情况②③，却是无效的。对此，需要借助左 - 右双旋转或右 - 左双旋转。

SearchTree：

（1）插入节点：将要插入的结点 e，与节点 root 节点进行比较，若小于则去到左子树进行比较，若大于则去到右子树进行比较，重复以上操作直到找到一个空位置用于放置该新节点。

（2）将要删除的节点的值，与节点 root 节点进行比较，若小于则去到左子树进行比较，若大于则去到右子树进行比较，重复以上操作直到找到一个节点的值等于删除的值，则将此节点删除。

a.删除节点不存在左右子节点，即为叶子节点，直接删除

b.删除节点存在左子节点，不存在右子节点，直接把左子节点替代删除节点。

c.删除节点存在右子节点，不存在左子节点，直接把右子节点替代删除节点。

d.删除节点存在左右子节点，则取左子树上的最大节点或右子树上的最小 节点替换删除节点。

（3）遍历：叉树的遍历是指从根结点出发，按照某种次序依次访问所有结点，使得每个结点仅被访问一次。共分为四种方式：前序遍历、中序遍历、后序遍历、层次遍历。

BTree：

（1）查找：若给定关键字等于结点中某个关键字Ki，则查找成功；若给定关键字比结点中的k1笑，则进入指针A0指向的下一层结点继续查找；若在两个关键字ki-1和ki之间，则进入它们之间的指针Ai指向的下一层结点继续查找；若比该结点所有关键字大，则在其最后一个指针An指向的下一层结点继续查找；若查找到叶子结点，则说明给定值对应的数据记录不存在，则查找失败。

（2）插入：利用查找过程查找关键字K的插入位置。若找到，则说明该关键字已经存在，直接返回；否则查找操作必失败于某个最底层的非终端结点上。在该结点中插入，若其关键字总数n未达到m，算法结束；否则，需分裂结点。

（3）分裂：生成一新结点，从中间位置把结点（不包括中间位置的关键字）分裂成两部分。前半部分留在旧结点中，后半部分复制到新结点中，中间位置的关键字连同新结点的存储位置插入到父结点中。如果插入后父结点的关键字个数也超过m-1，则要再分裂。这个向上分裂过程如果持续到根结点分裂为止，则会产生新的根结点。

（4）删除

a.如果被删除关键字所在结点的原关键字个数n>=(m/2)（向上取整），说明删除该关键字后该结点仍满足B树的定义。这种情况最为简单，只需从该结点中直接删除去关键字即可。

b. 如果被删除关键字所在结点的关键字个数n等于(m/2)（向上取整）-1，说明删除该关键字后该结点将不满足B树的定义，需要调整.调整过程为：如果其左兄弟结点中有“富余”的关键字，即与该结点相邻的右（或左）兄弟结点中的关键字数目大于(m/2)（向上取整）-1。则可将右（左）兄弟结点中最小（大）关键字上移至双亲结点。而将双亲结点中小（大）与该上移关键字的关键字下移至被删除关键字所在结点中。

c.如果相邻兄弟结点中没有富余的关键字，即相邻兄弟结点中的关键字数目均等于(m/2)（向上取整）-1。此时比较复杂，需把要删除关键字的结点与其左（或右）兄弟结点以及双亲结点中分割二者的关键字合并成一个结点，即在删除关键字后，该结点中剩余的关键字加指针，加上双亲结点中的关键字Ki一起，合并到Ai（即双亲结点中指向该删除关键字结点的左（右）兄弟结点的指针）所指的兄弟结点中去。如果因此使双亲结点中关键字个数小于(m/2)（向上取整）-1，则对此双亲结点做同样的处理。以至于可能直到对根结点做这样的处理而使整个树减少一层。

d.如果该结点不是最下层非终端结点，且被删关键字为该结点中第i个关键字key[i]，则可以从指针ptr[i]所指的子树中找出位于最下层非终端结点的最小关键字Y,替代key[i],然后在最底层非终端结点中移除Y。因此，把在非终端结点删除关键字k的问题就变成了删除最下层非终端结点中的关键字的问题了。