## Code - TODO: Comment and translate to C in q1()
.pos 0x1000

## C statement 1
S1:
                 ld    $i, r0            # r0 = &i
                 ld    (r0), r0          # r0 = i
                 ld    $a, r1            # r1 = &a memory address of pointer that points to a
                 ld    (r1), r1          # r1 = memory adress of struct a
                 ld    (r1), r1          # r1 = a->x[0]
                 ld    (r1, r0, 4), r2   # r2 = a->x[i]
                 ld    $v0, r3           # r3 = &v0 
                 st    r2, (r3)          # v0 = a->x[i]

## C statement 2
S2:
                 ld    $i, r0            # r0 = &i
                 ld    (r0), r0          # r0 = i
                 ld    $a, r1            # r1 = &a memory address of pointer that points to a
                 ld    (r1), r1          # r1 = base address of struct a
                 inca  r1                # r1 = base address of struct a + 4
                 ld    (r1, r0, 4), r2   # r2 = a->b.y[i]
                 ld    $v1, r3           # r3 = &v1
                 st    r2, (r3)          # v1 = a->b.y[i]

## C statement 3
S3:
                 ld    $i, r0            # r0 = &i
                 ld    (r0), r0          # r0 = i
                 ld    $a, r1            # r1 = &a
                 ld    (r1), r1          # r1 = a
                 ld    20(r1), r1        # r1 = a->b.a which is base address of a
                 ld    (r1), r1          # r1 = a->b.a->x[0]
                 ld    (r1, r0, 4), r2   # r2 = a->b.a->x[i]
                 ld    $v2, r3           # r3 = &v2
                 st    r2, (r3)          # v2 = a->b.a->x[i]

## C statement 4
S4:
                 ld    $a, r1            # r1 = &a
                 ld    (r1), r1          # r1 = base address of struct a
                 st    r1, 20(r1)        # a->b.a = a
## C statement 5
S5:
                 ld    $i, r0            # r0 = &i
                 ld    (r0), r0          # r0 = i
                 ld    $a, r1            # r1 = &a
                 ld    (r1), r1          # r1 = a memory address of struct a
                 ld    20(r1), r1        # r1 = a->b.a->x[0] memory address
                 inca  r1                # r1 = a->b.a->b.y[0]
                 ld    (r1, r0, 4), r2   # r2 = a->b.a->b.y[i]
                 ld    $v3, r3           # r3 = &v3
                 st    r2, (r3)          # v3 = a->b.a->b.y[i]


                 halt
.pos 0x2000
i:               .long 0x1                # # Globals
v0:              .long 0x0                
v1:              .long 0x0                
v2:              .long 0x0                
v3:              .long 0x0                
a:               .long 0x3000             
.pos 0x3000
d0:              .long 0x3030             # # Heap (these labels represent dynamic values and are thus not available to code)
                 .long 0x14               
                 .long 0x15               
                 .long 0x16               
                 .long 0x17               
                 .long 0x3018             
d2:              .long 0x3040             
                 .long 0x28               
                 .long 0x29               
                 .long 0x2a               
                 .long 0x2b               
                 .long 0x0                
d1:              .long 0xa                
                 .long 0xb                
                 .long 0xc                
                 .long 0xd                
d3:              .long 0x1e               
                 .long 0x1f               
                 .long 0x20               
                 .long 0x21               
