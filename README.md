# Video Analysis Tool (VAT)

The purpose of this software is to analyze video data to find information such as color space usage, image brightness statistics, and audio frequency response

## Requirements
`ffmpeg` libraries and `cairo`

## Building
```
git clone https://github.com/Pocketkid2/vat.git
cd vat
mkdir build
cd build
cmake ..
make
```

## Usage
`vat <module> <args>`

## Modules
| Module name | Abbreviated name | Description | Example |
| ----------- | ---------------- | ----------- | ------- |
| `bitrate` | `bt` | Analyzes the video bitrate and prints a PNG graph | `./vat bt ~/Videos/Test.mp4` |
