---
title: Hunter Pride
nav_order: 3
---

# Hunter Pride

Hunter Pride is Acheron's menu for interacting with defeated actors.

This page documents the behavior implemented in `HunterPride.cpp` and the Papyrus bridge in `Functions.cpp`.

## Built-In Options

- `rescue`: revive the defeated target
- `plunder`: open inventory/loot flow (treated as assault)
- `execute`: kill a defeated target
- `vampire`: feed option for vampires on valid targets (this action is treated as murder)

Built-in option IDs are fixed:

- `rescue` -> `0`
- `plunder` -> `1`
- `execute` -> `2`
- `vampire` -> `3`

## Adding Custom Options

Papyrus signature:

```papyrus
int Acheron.AddOption(string id, string name, string iconPath, string conditionJson)
```

Example:

```papyrus
int id = Acheron.AddOption(
    "MyOption",
    "$HP_CustomOption_Name",
    "Interface\\Acheron\\CustomIcon.dds",
    "{\"target\":{\"has\":{\"keywords\":[\"0xA82BB|Skyrim.esm\"]}}}"
)
```

Arguments:

- `optionId`: unique string key
- `name`: label shown in UI (localization key allowed)
- `icon`: icon path used by menu
- `conditions`: JSON condition string (optional)

Return value:

- `>= 0`: success
- `-1`: rejected (invalid/duplicate option)

## JSON Conditions

The bridge lowercases the entire JSON string before parsing.
Use lowercase keys and values.

Top-level branches:

- `player`
- `target`

Supported structure per branch:

```json
{
  "has": {
    "keywords": [],
    "factions": []
  },
  "not": {
    "keywords": [],
    "factions": []
  },
  "is": ["essential", "nonessential", "hostile", "nonhostile"]
}
```

Full example:

```json
{
  "player": {
    "has": {
      "keywords": ["0xA82BB|Skyrim.esm"]
    }
  },
  "target": {
    "not": {
      "keywords": ["0x13796|Skyrim.esm"]
    },
    "is": ["nonessential"]
  }
}
```

Form strings support both:

- `0xFORMID|Plugin.esp`
- raw numeric ID (hex with `0x` or decimal)

## Icon Paths

Supported file types are whichever the Scaleform UI consumes via the icon source string; defaults include `.swf` and custom options commonly use `.dds`/`.png`.

SWF frame syntax is supported:

```text
AcheronIcons.swf{Health}
AcheronIcons.swf{2}
```


### Handling Option Selection

When a custom option is selected, you receive an event through Papyrus. Register for the event and check the option ID:

```papyrus
Event OnInit()
    Acheron.RegisterForHunterPrideSelect(self)
EndEvent

Event OnHunterPrideSelect(int aiOptionID, Actor akTarget)
    If aiOptionID == Acheron.GetOptionID("MyOption")
        Debug.Trace("My option selected")
    EndIf
EndEvent
```

For aliases and magic effects, use:
- `RegisterForHunterPrideSelect_Alias(...)`
- `RegisterForHunterPrideSelect_MgEff(...)`

Event arguments:

- `aiOptionID`: selected option ID
- `akTarget`: interacted defeated actor

## Option Management

```papyrus
bool exists = Acheron.HasOption("MyOptionID")
int optionID = Acheron.GetOptionID("MyOptionID")
bool removed = Acheron.RemoveOption("MyOptionID")
```

## Notes

- Option IDs are unique.
- Custom option IDs start at `4`.
- Invalid condition/form parsing rejects option registration.
- The condition string is lowercased before parse.

## Last Updated

May 2026
