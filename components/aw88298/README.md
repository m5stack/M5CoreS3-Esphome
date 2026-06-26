# AW88298 — Class-D speaker amplifier (M5Stack CoreS3)

ESPHome component for the **AW88298** I2S class-D amplifier — the chip that drives
the speaker on the M5Stack CoreS3 (and CoreS3-Lite / SE). ESPHome has no built-in
driver for it, so audio output on these boards needs this component.

It manages only the **amplifier**; audio samples are fed separately by an
`i2s_audio` speaker on the same I2S bus.

## Behaviour

The amp is **power-managed**:

- At boot it stays **off** (enable rail low). The amp + boost converter draw
  current continuously when powered, which matters on battery.
- `power_on()` raises the enable rail, lets it settle, then runs the CoreS3
  enable / un-mute register sequence (boost → un-gate → sample-rate → volume).
- `power_off()` drops the rail again.

Call `power_on()` just before playback and `power_off()` when idle to avoid
draining the battery while silent. Both are idempotent.

## Configuration

```yaml
external_components:
  - source: github://m5stack/M5CoreS3-Esphome/components
    components: [aw88298]

i2c:
  sda: GPIO12
  scl: GPIO11

aw88298:
  id: speaker_amp
  # Must match the i2s_audio speaker's sample_rate (used to compute reg 0x06).
  sample_rate: 16000
  # Output pin gating the amp rail. On the CoreS3 this is AW9523 expander P0_2.
  enable_pin:
    aw9523b_id: aw9523b_hub
    number: 2
  # I2C address defaults to 0x36.
```

| Option | Default | Description |
|---|---|---|
| `sample_rate` | `16000` | I2S sample rate; must match the `i2s_audio` speaker. |
| `enable_pin` | — | Output pin gating the amp rail (CoreS3: AW9523 P0_2). |
| `address` | `0x36` | I2C address. |

## Driving it from playback

Bring the amp up around playback from your media-player automations, e.g.:

```yaml
media_player:
  - platform: i2s_audio
    # ...
    on_announcement:
      - lambda: "id(speaker_amp).power_on();"
    on_idle:
      - lambda: "id(speaker_amp).power_off();"
```

## Notes

- The register sequence matches the one M5Unified uses on the CoreS3.
- `enable_pin` must be driven before any I2C to the amp, or the chip won't ACK at
  0x36 — the rail powers it.
