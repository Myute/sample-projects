#############################################
# CS478: Assignment 6 - Group portion       #
#                                           #
# Desc: This program takes an image of      #
# receipt and translates it an organized    #
# text format using openCV and Tesseract    #
#                                           #
# File: Reader.py - Acts as main client     #
#                                           #
# Authors: Kevyn Higbee, Preston Roberts    #
# Date: May 12, 2021                        #
#############################################

import sys
from warnings import showwarning
import ShapeDetector as sd
import Parser as p
import cv2
import pytesseract
import re
import json
import os


def read(img,debug):
    pricepattern = r'^\S*\$*\d*(\.|,)\d{2}$' # for separating prices into separate list
    prices = []                             # we dont check for letters inside the price regex to
    words = []                              # avoid losing prices when numbers are recognized as letters
                                            # , is a brute force way to avoid losing data due to . being
                                            # recognized as ,
    priceColor=(255,0,0)
    wordColor=(0,0,255)

    readable = sd.getReadable(img,600,600,showImages=debug) # attempts to dewarp/transform image for better text recognition
    readable = sd.cleanImage(readable,showImages=debug)                      # further processing
    boxes=pytesseract.image_to_data(readable)               # read image text with pytesseract

    for x,b in enumerate(boxes.splitlines()):               # create 2 lists containing read text along with coordinates on image
        if x!=0:
            b=b.split()
            if len(b)==12:                                  # only parse entries that contain read text
                x,y,w,h = int(b[6]),int(b[7]),int(b[8]),int(b[9])   
                if re.findall(pricepattern,b[11]):          # separate entries into prices and words
                    prices.append(b)
                    color=priceColor
                else:
                    words.append(b)
                    color=wordColor
                cv2.rectangle(readable,(x,y),(w+x,h+y),color,1)                         # draw text location on image
                cv2.putText(readable,b[11],(x,y),cv2.FONT_HERSHEY_COMPLEX,0.5,color,1)  # the edited image is only shown if debug=True
    if debug:
        show = readable.copy()
        cv2.imshow('Detected Text',show)
        cv2.waitKey(0)

    return words,prices

def list2Dict(l):
    d = {}
    for i in l:
        d.update({i[0]:i[1]})
    return d

def toJSON(l1, l2, name):
    d1 = list2Dict(l1)
    d2 = list2Dict(l2)
    outDict = {'Billing Information':d1,'Charges':d2}
    jsonStr = json.dumps(outDict)

    print(jsonStr)

    f = open("Output/"+name,'w')
    f.write(jsonStr)
    f.close()
    return

def main():
    if len(sys.argv) < 1:   # make sure we have arguments, does not check if valid filepath
        print("No receipts were specified to be read.")
        return

    sys.argv.pop(0)         # make argv[0] filepath
    path = sys.argv[0]
    sys.argv.pop(0)

    debug = False
    if len(sys.argv) > 0:   # if show is specified set debug=True
        if sys.argv[0] == 'show':
            debug = True

    img = cv2.imread(path)
    words,prices = read(img,debug)          # get data from image
    word_res = p.findKeyWords(words,prices) # search for keywords and their respective prices values
    price_res = p.findItems(words,prices)   # search for any other charges by prices

    print(word_res)                         # print output, this needs to be in a different format
    print(price_res)

    toJSON(word_res,price_res, (os.path.splitext(os.path.basename(path))[0]+'.txt'))

main()