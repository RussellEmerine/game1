# Gravity Race

### Author: Russell Emerine

### Design: 
There are games that use gravity to fixed points as a movement mechanic. 
Gravity Race instead uses gravity to freely moving points, which makes more complex interaction.

### Screen Shot:

![Screen Shot](screenshot.png)

### How Your Asset Pipeline Works:

The tiles are read from a single `atlas.png` file:

![Screen Shot](atlas.png)

Most of the tiles are a font drawn in GIMP, very loosely inspired by
[damieng's Space Opera](https://damieng.com/typography/zx-origins/space-opera/).
The tiles are numbered left to right, top to bottom.
Due to the game's simple color scheme, only two colors are needed to encode the tiles.

The tiles are ordered so that text characters coincide with their ASCII numbers.
`atlas.png` is loaded in, and each tile is read pixel by pixel.
This only happens once, since all game modes use the same tiles.

Levels are represented using a simple text format described in 
[`levels/README.md`](levels/README.md).
This does involve parsing at runtime, which is slow,
but this also allows for users to easily make their own levels.
At runtime, the `levels` directory is traversed and any `.lvl` files
are treated as level files and presented at select screen.
This means users can add or modify levels at will
and only have to restart the game rather than rebuild and reinstall.

### How To Play:

You are a ball, and there are other balls.
Balls of the same color share the same gravity,
which means all red balls are attracted to each other,
and all green balls are attracted to each other,
but red and green balls are not attracted to each other.
You can change your ball's color at will using the number keys `1` `2` `3` `4` `5` `6` `7` `8`.
(There are always 8 colors.)
Try to reach a goal area as fast as possible.

This game was built with [NEST](NEST.md).

