---
title: Events
nav_order: 2
---

# Events

Events define which quest Acheron starts after a defeat scenario.

This page documents the schema used by `EventData` and `Resolution`.

## File Location

Event files are loaded from:

```text
Data/SKSE/Acheron/Consequences/<Type>/
```

Supported folders:

- `Hostile`
- `Follower`
- `Civilian`
- `Guard`
- `NPC`

Notes:

- Files must end in `.yml` or `.yaml`.
- Two built-in defaults are added by code for Hostile and Guard before file loading.

## Minimal Event

```yaml
Version: 2
Name: "Captured"
Quest: 0x800|MyMod.esp
```

## Event YAML Shape (Version 2)

```yaml
Version: 2

Name: "Event Name"
Description: "Shown in MCM"
Quest: 0xABC|MyMod.esp

Priority: 1
Cooldown: 0
Teleport: true
StartTeleport: false
InCombat: false
Hidden: false

Requirements:
  - MyDependency.esp

Conditions:
  Assailant:
    RaceType:
      - Human
    Faction:
      - 0x0005C84D|Skyrim.esm: true
    Keyword:
      - 0x0006BBE8|Skyrim.esm: true
    CrimeFaction:
      - None

  Victim:
    Faction:
      - 0x0005C84D|Skyrim.esm: false
    Keyword:
      - 0x000A82BB|Skyrim.esm: false
    Location:
      - 0x00018A34|Skyrim.esm: true
    Worldspace:
      - 0x0000003C|Skyrim.esm: true
    CrimeFaction:
      - None

  Quest:
    Running:
      - 0x00012345|MyMod.esp: true
    Completed:
      - 0x00012345|MyMod.esp: false
```

## Field Reference

| Field | Required | Implementation Details |
|---|---|---|
| `Version` | yes | Version of the parser to use. Copy from reference guide. |
| `Name` | no | Defaults to `UNTITLED`. |
| `Quest` | yes | Must resolve to a quest form. |
| `Requirements` | no | Sequence of plugin names; any missing plugin rejects the event file. |
| `Cooldown` | no | Parsed and stored for event data. |
| `Priority` | no | Version 2 accepts only `0..3`; invalid values reject file. |
| `Teleport` | no | Flag on event. |
| `StartTeleport` | no | If true, Teleport is forced on as well. |
| `InCombat` | no | Flag on event. |
| `Hidden` | no | Hidden events are omitted from MCM lists. |

## Condition Value Formats

Condition entries can be either:

1. Scalar value (implicit `true`):

```yaml
Faction:
  - 0x0005C84D|Skyrim.esm
```

2. Value + bool compare:

```yaml
Faction:
  - 0x0005C84D|Skyrim.esm: false
```

For form-based types, `none` or `null` are accepted and resolved to `nullptr`.

## Condition Branches

- `Assailant` conditions: each condition must match at least one aggressor in the list.
- `Victim` conditions: all must match the victim.
- `Quest` conditions: all must match globally.

Supported keys:

- `Assailant`: `RaceType`, `Faction`, `Keyword`, `CrimeFaction`
- `Victim`: `Faction`, `Keyword`, `Location`, `Worldspace`, `CrimeFaction`
- `Quest`: `Running`, `Completed`

## Selection and Priority Behavior

When selecting an event:

1. Invalid events are filtered out (flags, conditions, zero/negative weight).
2. Events are grouped by priority.
3. Highest non-empty priority group is processed first.
4. Weighted random draw is applied.
5. If a quest fails to start, it is removed and draw repeats.

## Weights

Weights are not read from event YAML files.

They are loaded/saved via:

```text
Data/SKSE/Acheron/Consequences/Weights.yaml
```

If no weight is defined, default is `50`.

## Legacy Version 1 Notes

Version 1 is still parsed for compatibility:

- priority uses old numeric mapping to internal tiers
- legacy root-level condition keys are used

For new content, use Version 2 format only.

## Common Mistakes

- Putting events in wrong folder name
- Using non-quest form in `Quest`
- Using invalid `Priority` for version 2
- Assuming `Weight` is read from event YAML
- Missing `Requirements` plugins

## Last Updated

May 2026
