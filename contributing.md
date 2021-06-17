# Contributing to dahlia
All contributions to dahlia are very welcome and needed. If you have decided to contribute to this project, all you have to do is follow a few minor guidelines.

## Commit messages
When writing a commit message, please make sure it is in the format of
```
category: what you did here...
```
For example, if I fixed a bug in the `printk` function under `tty`, my commit message would be something like this
```
tty: printk bugfixes
```
You can go into more detail, but make sure your commit messages aren't too long. While you are at it, make sure that the commits themselves aren't long. Split up large commits into smaller ones so that we don't end up with another horror like this [one](https://github.com/synthels/dahlia/commit/51416efe92011e22a2f18008b4edc683bf8d8d42)...

## Code style
I ask that you follow our code style, since it makes the codebase more pretty and organized. First of all, we only use tabs. Please indent your code with tabs and not spaces so things don't get confusing.

### Functions
All of the function singature must be on the same line, only putting the opening brace on the next.

```c
void foo(int p1, int p2)
{
	/* ... */
}
```

### Comments
Please only write comments like this:

```c
/* Good comment! */
```

Not like this:

```c
// Bad comment :(
```

### If statements and while/for loops
All if statements, for and while loops must be written with a space left between the for/while or if and the opening parenthesis. The opening brace must be put on the same line.

```c
for (int i = 0; i < size; i++) {
	/* ... */
}
```

### Header files
Header files must begin with a copyright header, like this

```c
/*
 * Copyright (C) 2020 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
```

You also need the same header on source files, like this:

```c
/*
 * Copyright (C) 2020 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * Short description of code
 */
```

All header files must follow C convention, leaving us with a format like this:

```c
/*
 * Copyright (C) 2020 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef NAME_OF_FILE
#define NAME_OF_FILE

/* All includes go here... */

#endif
```
