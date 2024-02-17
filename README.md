# Video Analysis Tool (VAT)
The purpose of this software is to analyze video data to find information such as color space usage, image brightness statistics, and audio frequency response

## Usage
`vat <tool> <video file>`

## Tools
| Tool name | Abbreviated name | Description |
| --------- | ---------------- | ----------- |
| `analyze_color_space` | `acs` | Scans the entire video file and looks at the color space usage compared to the container maximum |
| `analyze_brightness` | `ab` | Scans the entire video file and looks at the brightness usage compared to the container maximum |
| `analyze_audio` | `aa` | Scans the entire first audio track and prints a graph of the frequency response |
