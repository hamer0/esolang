# Mini Esolang
A simple esolang based on single character instructions that manipulate a sliding window.

A goal of this language is to make the interpreter small without making it unreadable. I'm not too fussy about the time complexity of the interpreter, I just want to make the code as concise as I can. For this reason there are steps could be done more efficiently.

## Notes
- Currently only has 32 bits of memory
- Instruction set is very limited
- I haven't decided on a name or file extention so I hit some random keys and got `zxb1`. The file extension and executable are both therefore `zxb1`

## Instruction Set

- [x] `>` shift window right
- [x] `<` shift window left
- [x] `+` grow window 1 bit
- [x] `-` shrink window 1 bit
- [x] `1` set window
- [x] `0` clear window
- [x] `~` flip bits in window
- [x] `@` output as char
- [ ] `#` output binary
- [x] `?` skip next char if window = 0

- [x] `(` loop block begin
- [x] `)` loop block end
- [ ] `{` block begin
- [ ] `}` block end
  
- [x] `"  "` comment

## Window

The window encapsulates the current area of memory that can be manipulated. If you know pointers, think of the left and right bounds as 2 pointers (because they are)

Given a 16 bit memory space, the window could look like this:
```
0000 [0000] 0000 0000
```
where `[]` surrounds the encapsulated bits by the 2 pointers. See how different instructions affect the window:

**Shift Right** `>`
Shifts the window 1 bit the right
```
0000 0[000 0]000 0000
```

**Shift Left** `<`
Shifts the window 1 bit the left

```
000[0 000]0 0000 0000
```

**Grow** `+`
Expands the window (from right) 1 bit
```
0000 [0000 0]000 0000
```

**Shrink** `-`
Shrinks the window (from right) 1 bit
```
0000 [000]0 0000 0000
```

**Set** `1`
Sets all bits within the window
```
0000 [1111] 0000 0000
```
