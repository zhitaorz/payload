// Per-process state
struct procf {
  uint sz;                     // Size of process memory (bytes)
  enum procstate state;        // Process state
  int pid;                     // Process ID
  int parent;                  // Parent process ID
  char name[16];               // Process name (debugging)
  uint nsp;                    // the namespace number
};