# ReceiptReader
CS 478 Final Project Files

## About this Project
ReceiptReader is a python project that extracts data from an image of a receipt and outputs the data in an organized json format. Text extraction is accomplished using pytesseract
and image processing is accomplished using opencv.

Data is organized by location of the text on the receipt.
## Usage
### Prerequisites
This project requires tesseract and opencv to be installed and was tested in Python 3.9 .
```sh
  pip install tesseract
  pip install opencv-python
```
### How to Use
```sh
  python Reader.py 'filename' -show
```
The '-show' flag will display images of the receipt along each step of image preprocessing.

## Known Issues
Current known issues in this project
* Cropped Images do not maintain aspect ratio
* Regex leaves lots of room for errors
* Does not that text is upright before reading

This project is currently shelved and these issues are not planned to be fixed.
