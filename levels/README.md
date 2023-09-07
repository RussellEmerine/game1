A `.lvl` file is a text file with a simple syntax.
The syntax is as follows. All colors are hex values (without prefix),
ball groups are integers from 0 to 7 inclusive,
and all ball positions and velocities are decimals.
Currently, errors in file format are not detected,
and only ASCII filenames and contents are supported.

line 1: single three-byte color for the background

lines 2-9: single four-byte color for the palettes, in order

lines 10-39: lines of characters representing tiles

- the following characters represent special tiles and cannot be used as display characters
    - ` ` (space) represents empty tile (id 0)
    - `#` represents filled tile (id 1)
    - `@` represents goal (id 16)
- other characters represent themselves as text
- lines longer than 32 characters are truncated
- lines shorter than 32 characters are filled with blank tiles

lines 40-end: five numbers representing a ball

- as in `group pos.x pos.y vel.x vel.y`
- there should be at least one ball, and the first is the player ball

Due to the simplicity of the `.lvl` format,
you can easily edit existing levels or create your own levels.
The `.lvl` file is a good way to preview a level while you're designing it,
since the layout section is visually accurate with a monospace font.
