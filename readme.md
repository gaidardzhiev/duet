# duet

Minimalist dual pane terminal file manager via extreme C preprocessor abuse and pathological macros [duet.c](./duet.c)

```
------------------------------------------------------
| /home/tmp/duet          | /home/tmp/duet/duet      |
-----------------------------------------------------|
| duet                    | .ELF..............(.....P|
| duet.c                  | ..4...........4. .       |
| Makefile                | .(........px...x...x.....|
| readme.md               | ....4...4...4...@...@....|
------------------------------------------------------
 1/4  hjkl/arrows:nav  .:hidden  TAB:fzf  !:shell  q:quit
```

Left pane navigates the filesystem. Right pane shows a live preview of whatever is under the cursor: directory contents, text file content, or a sanitized binary dump for executables. When you move the cursor the preview updates instantly with no flicker.

## Build

```
make
```

Or directly:

```
gcc -o duet duet.c
```

No dependencies beyond libc and standard POSIX headers.

## Install

```
make install
```

## Usage
| Key                   |  Action                                   |
|-----------------------|-------------------------------------------|
|`hjkl` or `arrow keys` |  for navigation                           |
|`h` or `<`             |  go to parent directory                   |
|`l` or `>` or `Enter`  |  enter directory or open file in $EDITOR  |
|`g`                    |  jump to top                              |
|`G`                    |  jump to bottom                           |
|`.`                    |  toggle hidden files                      |
|`~`                    |  go to $HOME                              |
|`TAB` or `/`           |  fuzzy find with fzf                      |
|`!`                    |  spawn a $SHELL                           |
|`q` or `Ctrl+C`        |  to quit                                  |

## File preview

The right pane previews whatever is under the cursor.

**Directories** show their contents, color coded the same way as the left pane.

**Text files** show the first screenful of content with tabs expanded and long lines clipped to the pane width.

**ELF binaries and executables** show a sanitized dump of the first 4096 bytes. Non printable bytes are replaced with `.` so the file stays readable, section headers, linker paths, and symbol names bleed through naturally.

## Color coding

Directories are **blue**. Executables and ELF binaries are **green**. Regular files are plain.

## How does it work

Correctly.

## License

GPL-3.0-only Copyright (C) 2026 Ivan Gaydardzhiev
