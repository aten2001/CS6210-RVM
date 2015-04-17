Recoverable Virtual Memory
- Harshit Chawla
- Akshar Rawal


Transaction implementation
1. Transactions operate on memory segments and the segments have to be initialized before a transaction operated on it.
2. Each transactional process starts with the rvm_init() function that initializes/opens the log directory for the segment involved in the transaction. It also instantiates the redo log file within the directory.
3. Once initialized, the segment is mapped to a physical memory location using the rvm_map() function.
4. This is where the actual transactional code can begin execution on the segment.
5. Transactions are initialized using the rvm_begin_trans() function.
6. When the transaction starts, the segments involved in the transaction are checked if they already are being used with a different transaction that is running. This resolves the issue of deadlocks by terminating the transactions trying to operate on the same segment.
7. The “begin transaction” checkpoint also adds the new transaction to the list of transactions currently executing.
8. When the transaction starts, all data in the redo log, for the segments involved in this transaction, is copied over into external buffers for commit to external buffers. The new segment is then instantiated by mapping the current transaction.
9. Whenever a transaction is about to modify a segment, an entry is created into the log file with a pointer to the current region pointer. This allows to undo the transaction/process until this point, before the changes that follow.
10. On completing all required steps a transaction can commit with the rvm_commit_trans() function.


Commit Protocol
1. Before committing the changes into memory, the segments that were modified by the current transaction are written into the redo log file. This helps the transaction to be redone if there is a system crash while writing the changes to memory.
2. The data is logged with the TRANSACTION START and TRANSACTION END markers.
3. The undo logs for the transaction are them removed as the transaction has successfully committed.
4. The transaction can now be removed from the transaction list.
5. The is performed for all segments that the transaction operates on.


Abort Protocol
1. When an abort is called on a transaction, all segments related to that transaction are released.
2. The undo log for the transaction is removed as the transaction has to restart.
3. The memory regions initialized for the transaction are also released.
4. The transaction is removed from the list of transactions.
5. This is performed over all segments held by the transaction.


Log Files
1. Two log files are maintained, Undo log and Redo log.
2. The undo log file is maintained per region, whereas the redo log file is maintained per transaction per segment.
3. Undo log files store the last known commit/save information for a transaction/process during execution. In case of a transaction failure/abort, this log is used to start the transaction over.
4. Redo logs store all successful changes performed by a transaction/process. In case of a system crash while committing to memory, this log can be used to retrieve the changes made by the transaction.
5. Every time that a transaction commits, the undo log files are flushed as all data exists in the redo log file.
6. When a new transaction begins operating on a segment, the redo log for that segment is flushed and the data within the logs is written to external buffers.
7. This ensures that the log file sizes don’t keep expanding infinitely.
8. All logs are marked appropriately with tags like:
   1. TRANSACTION START
   2. TRANSACTION END
   3. REGION START
   4. REGION END
   5. SEGNAME
   6. OFFSET
   7. SIZE
   8. DATA
1. All logs contain the segment, the region and the data manipulated by a transaction.