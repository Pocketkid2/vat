# Video Analysis Tool (VAT)

The purpose of this software is to analyze video data to find information such as color space usage, image brightness statistics, and audio frequency response

## Usage
`vat <module> <args>`

## Modules
| Module name | Abbreviated name | Description |
| --------- | ---------------- | ----------- |
| `analyze_color_space` | `acs` | Scans the entire video file and looks at the color space usage compared to the container maximum |
| `analyze_picture_brightness` | `apb` | Scans the entire video file and looks at the brightness usage compared to the container maximum |
| `analyze_audio_track` | `aat` | Scans the entire audio track and prints a graph of the frequency response |
