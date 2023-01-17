ld $8, r1  # r1 = size 
gpc $4, r0 # r0 = buffer (exec code at address after halt)
sys $2     # system call exec
halt
.long 0x2f62696e   # load hex /bin into memory 
.long 0x2f736800   # load hex /sh alongside