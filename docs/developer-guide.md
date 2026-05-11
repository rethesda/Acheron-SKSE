---
title: Developer Guide
nav_order: 6
---

# Developer Guide

This page is the quick workflow view for mod authors integrating with Acheron.

## Workflow 1: Add a Consequence Event

1. Create a quest that contains your consequence logic.
2. Add a YAML event file under:

```text
Data/SKSE/Acheron/Consequences/<EventType>/
```

3. Point `Quest` to your quest form.
4. Add conditions and priority.

Minimal example:

```yaml
Version: 2
Name: "My Event"
Quest: 0x800|MyMod.esp
Priority: 1
```

## Workflow 2: Add a Hunter Pride Action

1. Register a custom option with `AddOption`.
2. Register for Hunter Pride select events.
3. Handle `OnHunterPrideSelect`.

```papyrus
Event OnInit()
    int id = Acheron.AddOption("MyOption", "$MyOptionName", "MyIcon.dds", "{}")
    Acheron.RegisterForHunterPrideSelect(self)
EndEvent

Event OnHunterPrideSelect(int aiOptionID, Actor akTarget)
    If aiOptionID == Acheron.GetOptionID("MyOption")
        ; your logic
    EndIf
EndEvent
```

## Workflow 3: Guard Fragile Quest Segments

Use API toggles when a quest section is too fragile for defeat replacement:

```papyrus
Acheron.DisableProcessing(true)
; critical scene
Acheron.DisableProcessing(false)
```

Or use YAML exclusions in `Data/SKSE/Acheron/Validation/` for persistent rules.

## Recommended Test Pass

- Defeat path triggers expected event
- Event does not trigger in excluded cases
- Hunter Pride option appears only when valid
- Invalid conditions fail safely (no crash)
- Save/load keeps expected defeat state

## Common Pitfalls

- Wrong form string format: use `0xFORMID|Plugin.esp`
- Missing plugin from `Requirements`
- Wrong event type folder (`Hostile`, `Follower`, `Civilian`, `Guard`, `NPC`)
- Option ID collisions between mods

## Where to Go Next

- Event schema: [Events](Events.md)
- Hunter Pride options: [Hunter Pride](Hunter-Pride.md)
- API reference: [Papyrus API](Papyrus-API.md)
- Exclusions and safety: [Validation](Validation.md)

## Last Updated

May 2026
