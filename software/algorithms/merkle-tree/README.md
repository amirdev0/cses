Merkle Tree
===========

This is a primitive merkle tree implementation via heap-allocated array in C.

## Implementation
Building merkle tree and proof have O(n) and O(log(n)) space-time complexity respectively.

Merkle tree, which is a type of binary tree, is implemented via heap-allocated flat array.
This approach allows the implementation support large input and enjoy spatial locality.
Its nodes are structures with minimal, yet critical metadata like node index and level.
The required array allocated for merkle tree hashes is kept minimal, without paddings.
It also has metadata like tree height and nodes count.  


## Use Case
Merkle tree is usually used in blockchains as it allows to speed-up verification process of transactions for devices with limited raw processing power (like smartphones).
Bitcoin blockchain, which is simply linked list of blocks, is huge, because of new blocks being added every 10 minutes since 2009, with every block around 2MB in size.
Usually Merkle tree for a Bitcoin block is built several times if the resulting hash, including Merkle root, doesn't satisfy target value (set by Bitcoin algorithm).

To keep Bitcoin wallet updated, light-node (like Bitcoin software on smartphone) doesn't download full Bitcoin blockchain and traverse every transaction in every block. 
Instead, it downloads only header of the block (80 bytes in size each) and requests full-node in Bitcoin network a merkle proof for verification of specific transaction.
Merkle proof is simply a list of sibling hashes (forming path from leaf to the root) of merkle tree and its length is log2(n), where n is the number of transactions.
Thus, for a block with 4000 transactions light-node has to perfrom only 12 operations instead of 4000 (at most, because transaction can be somewhere in the middle).  

Also it is woth mentioning that light-node (or SPV Wallet) is also different from full-node in a way it is notified about transactions related to its address.
