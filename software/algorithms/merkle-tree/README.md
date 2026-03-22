Merkle Tree
===========

Merkle tree implementation with unit tests and benchmarks.  

## Implementation
Building merkle tree and proof have O(n) and O(log(n)) space-time complexity respectively.

Merkle tree (specific type of binary tree) implementation is:
- heap-allocated flat-array - to support large values and enjoy spatial locality
- not padded - to make memory allocated only for existing nodes (without expanding capacity though)


## Use Case
Merkle tree is actively used in blockchains as it allows to speed-up verification process of transactions for devices with limited raw processing power (like smartphones).
Bitcoin blockchain, which is simply list of blocks, is huge, because of new blocks being added every 10 minutes since January 2009, with every block around 2MB in size.
Algorithm for constructing Merkle tree used by miner for a Bitcoin block always places first its reward transaction and then transactions from mempool in topological order.
If the resulting Block hash doesn't satisfy the target value (set by Bitcoin algorithm) and all nonce vales were exhausted, Merkle tree is rebuilt with other transactions.  

To keep Bitcoin wallet updated, light-node (like Bitcoin software on smartphone) doesn't download full Bitcoin blockchain and traverse every transaction in every block.
Instead, it downloads only header of each block (80 bytes in size each) and requests full-node in Bitcoin network a Merkle proof to verify transaction to its address.
Merkle proof is simply a list of sibling hashes (forming path from leaf to the root) of merkle tree and its length is log2(n), where n is the number of transactions.
Thus, for a block with 4000 transactions light-node has to perfrom only 12 operations instead of 4000 (at most, because transaction can be somewhere in the middle).  

