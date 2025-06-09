# A table showing the keymappings of the XBee remote control.

> [!IMPORTANT]
> Certain keymappings are case sensitive. Some are only available when the Zumo is in manual control mode (see Condition in table below).

| Input Key | Action | Condition | Note |
|-----------|--------|-----------| ---|
|`W` | Move forward | Manual control mode | Case insensitive |
|`S` | Move backward | Manual control mode | Case insensitive |
|`A` | Turn left | Manual control mode | Case insensitive |
|`D` | Turn right | Manual control mode | Case insensitive |
|` ` or `0`| Stop | Manual control mode | Blank (eg spacebar) |
|`X` | Print Diagnostics to XBee remote | None | Case insensitive |
|`+` or `=` | Increase speed | Manual control mode | Prints the new speed to XBee |
|`-` or `_` | Decrease speed | Manual control mode | Prints the new speed to XBee |
|`M` | Reset speed to default | Manual control mode | Case insensitive |
|`H` | Print an overview of all commands</br> for the current mode of operation | None | Case insensitive |
|`P` | Start linefollowing | None | Case insensitive |
|`O` | Stop linefollowing | None | Case insensitive |


---

**Printable table for Serial1 (Arduino C++):**

```cpp
Serial1.println(F("Input Key | Action                       | Condition           | Note"));
Serial1.println(F("----------+------------------------------+---------------------+-----------------------------"));
Serial1.println(F("W         | Move forward                 | Manual control mode | Case insensitive"));
Serial1.println(F("S         | Move backward                | Manual control mode | Case insensitive"));
Serial1.println(F("A         | Turn left                    | Manual control mode | Case insensitive"));
Serial1.println(F("D         | Turn right                   | Manual control mode | Case insensitive"));
Serial1.println(F("  or 0    | Stop                         | Manual control mode | Blank (eg spacebar)"));
Serial1.println(F("X         | Print Diagnostics to XBee    | None                | Case insensitive"));
Serial1.println(F("+ or =    | Increase speed               | Manual control mode | Prints new speed to XBee"));
Serial1.println(F("- or _    | Decrease speed               | Manual control mode | Prints new speed to XBee"));
Serial1.println(F("M         | Reset speed                  | Manual control mode | Prints new speed to XBee"));
Serial1.println(F("H         | Print overview of commands   | None                | Case insensitive"));
Serial1.println(F("P         | Start linefollowing          | None                | Case insensitive"));
Serial1.println(F("O         | Stop linefollowing           | None                | Case insensitive"));
```
