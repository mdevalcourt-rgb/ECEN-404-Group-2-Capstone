# DMM — Current

Use this mode to measure DC current through a branch of your circuit. Current is always measured **in series** — the MADAM3 must be inserted into the current path.

## Pins Used

| Pin | Location | Function |
|---|---|---|
| **Current+** | Middle header, pin 3 | Current input — connect to the more positive side |
| **Current−** | Middle header, pin 4 | Current output — connect to the more negative side |

## Wiring

1. **Break** the circuit at the point where you want to measure current. Remove the wire that carries the current you want to measure.
2. Connect **Current+** to the side of the break closer to the positive voltage source.
3. Connect **Current−** to the side of the break closer to ground.
4. The circuit is now complete through the MADAM3's internal current sense path.
5. Read the current from this screen.

## Example

To measure current through a load resistor R\_L:

```
Before:  ... ──── R_L ──── ...
After:   ... ── Current+ ── MADAM3 ── Current− ── R_L ── ...
```

Current flows in through Current+ and out through Current−.

## Safety

- The MADAM3 current measurement supports up to **±5 A**.
- **Never** connect Current+ and Current− directly across a voltage source. This creates a near-short-circuit.
- Only insert the current measurement into a branch that already has a load limiting the current.
