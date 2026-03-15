Merkle Tree
===========

This is a primitive merkle tree implementation in C, implemented via heap-allocated array.


## Use Case
Merkle tree is usually used in blockchains as it allows to speed-up verification process of transactions for devices with limited raw processing power (like smartphones).
Bitcoin blockchain, which is simply linked list of blocks, is huge, because of new blocks being added every 10 minutes since 2009, with every block around 2MB in size.  

To keep Bitcoin wallet updated, light-node (simply Bitcoin software i.e. on smartphone) don't traverse every transaction in every block in downloaded Bitcoin blockchain. 
Instead, it downloads only header of the block (80 bytes in size each) and requests (full-node in) Bitcoin network merkle proof for verification of specific transaction.
The provided Merkle proof is simply a list of paired sibling hashes - forming path from leaf to the root - of merkle tree and its length is log2 of transactions number.
Thus, for a block with 4000 transactions light-node has to perfrom only 12 operations insted of 4000 (at most, because transaction can be somewhere in the middle).  

Also it is woth mentioning that light-node (or SPV Wallet) is also different from full-node in a way it is notified about transactions related to its address.
