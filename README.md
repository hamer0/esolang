# Mini Esolang
A simple esolang based on single character instructions that manipulate a sliding window.

A goal of this language is to make the interpreter small without making it unreadable.

## Notes
- Currently only has 32 bits of memory
- Instruction set is very limited
- I haven't decided on a name or file extention so I hit some random keys and got `zxb1`. The file extension and executable are both therefore `zxb1`

## Instruction Set

- [x] `+` increment window
- [x] `-` decrement window
- [x] `>` extend window 1 bit
- [x] `<` shrink window 1 bit
- [x] `/` set window
- [x] `\` clear window
- [x] `@` output as char
- [ ] `#` output binary
- [ ] `}` Jump back to matching `{`
- [ ] `?` skip next char if window = 0

## Window

The window encapsulates the current area of memory that can be manipulated. If you know pointers, think of the left and right bounds as 2 pointers (because they are)

Given a 16 bit memory space, the window could look like this:
```
0000 [0000] 0000 0000
```
where `[]` the encapsulated bits by the 2 pointers. See how different commands affect the window:

**Increment** `+`
Shifts the window 1 bit the right
```
0000 0[000 0]000 0000
```

**Decrement** `-`
Shifts the window 1 bit the left

```
000[0 000]0 0000 0000
```

**Expand** `>`
Expands the window (from right) 1 bit
```
0000 [0000 0]000 0000
```

**Shrink** `<`
Shrinks the window (from right) 1 bit
```
0000 [000]0 0000 0000
```

**Set** `/`
Sets all bits within the window
```
0000 [1111] 0000 0000
```