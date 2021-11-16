#############################################
# CS478: Assignment 6 - Group portion       #
#                                           #
# Desc: This program takes an image of      #
# receipt and translates it an organized    #
# text format using openCV and Tesseract    #
#                                           #
# File: ShapeDetector.py                    #
# - Uses openCV to create a more readable   #
#   image                                   #
#                                           #
# Authors: Kevyn Higbee, Preston Roberts    #
# Date: May 12, 2021                        #
#############################################

import cv2
import numpy as np
import deskew
from typing import Tuple, Union
import math

def getReadable(img, width, height,showImages=False):
    myImg = img.copy()
    myImg = cv2.resize(myImg,(width,height),interpolation=cv2.INTER_CUBIC)  # this needs to be changed to some way that maintains aspect ratio
    img_proc = getProcessed(myImg,showImages)    # process image
    largestCont = largestContour(img_proc,showImages)
    if largestCont is None:
        return img  # unable to recognize receipt edges, return original image to be read in main
    newPoints = reorder(largestCont)
    img_warped = warpImage(myImg,newPoints,width,height,showImages)
    return img_warped

def getProcessed(img,showImages):
    myImg = img.copy()
    img_g = cv2.cvtColor(myImg,cv2.COLOR_RGB2GRAY)
    img_bi = cv2.bilateralFilter(img_g,7,50,50)
    edges = cv2.Canny(img_bi,30,100) # edge detection with canny

    if(showImages):
        stacked = np.hstack((img_g,img_bi,edges))
        cv2.imshow('Processed Images',stacked)
        cv2.waitKey(0)
    return edges

def largestContour(img,showImages):
    contours,hierarchy = cv2.findContours(img,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    img_cnt = img.copy()
    img_cnt = cv2.drawContours(img_cnt,contours, -1, (255,100,100), 3)
    
    # approximate all contours greater than min size
    img_bound = img.copy()
    maxArea = -1
    biggest = None
    for cont in contours:
        area = cv2.contourArea(cont)
        if area > 1000:
            cv2.drawContours(img_bound,cont,-1,(255,0,0),3)
            peri=cv2.arcLength(cont,True)
            approx=cv2.approxPolyDP(cont,0.02*peri,True)
            if len(approx)==4 and area > maxArea:
                biggest=approx
                maxArea=area

    if biggest is not None:
        if showImages:
            img_corners = cv2.drawContours(img.copy(),biggest,-1,(255,0,0),20)
            cv2.imshow('Detected Receipt Corners', img_corners)
            cv2.waitKey(0)
        return biggest
    else:
        print("WARNING: Unable to find receipt edges, Text recognition is less likely to be accurate.")
        return None

def reorder(points):
    points = points.reshape((4,2))
    pointsnew = np.zeros((4,1,2),np.int32)
    add = points.sum(1)
    pointsnew[0] = points[np.argmin(add)]
    pointsnew[3] = points[np.argmax(add)]
    diff = np.diff(points,axis=1)
    pointsnew[1] = points[np.argmin(diff)]
    pointsnew[2] = points[np.argmax(diff)]
    return pointsnew


def warpImage(img, points, w, h, showImages):
    warped = img.copy()
    p1 = np.float32(points)
    p2 = np.float32([[0,0],[w,0],[0,h],[w,h]])
    matrix = cv2.getPerspectiveTransform(p1,p2)
    warped=cv2.warpPerspective(warped,matrix,(w,h))

    if showImages:
        cv2.imshow('Transformed Image', warped)
        cv2.waitKey(0)
        
    return warped

# attempts to do final rotations of image to correct slanted text
def correctSkew(originalImg):
    img = originalImg.copy()
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    ang = deskew.determine_skew(gray)
    rotated = rotate(img,ang,(0,0,0))

    return rotated

# function taken from https://libraries.io/pypi/deskew
def rotate(image: np.ndarray, angle: float, background: Union[int, Tuple[int, int, int]]) -> np.ndarray:
    old_width, old_height = image.shape[:2]
    angle_radian = math.radians(angle)
    width = abs(np.sin(angle_radian) * old_height) + abs(np.cos(angle_radian) * old_width)
    height = abs(np.sin(angle_radian) * old_width) + abs(np.cos(angle_radian) * old_height)

    image_center = tuple(np.array(image.shape[1::-1]) / 2)
    rot_mat = cv2.getRotationMatrix2D(image_center, angle, 1.0)
    rot_mat[1, 2] += (width - old_width) / 2
    rot_mat[0, 2] += (height - old_height) / 2
    return cv2.warpAffine(image, rot_mat, (int(round(height)), int(round(width))), borderValue=background)

def getScaleRatio(img):
    minsize = 600
    # if #rows (height) > #cols (width)
    scale=(img.shape[1] if img.shape[1] < img.shape[0] else img.shape[0]) / minsize
    return scale if scale > 1 else 1

def cleanImage(img,showImages=False):
    cleaned = img.copy()
    cleaned = correctSkew(img)
    # this doesnt do anything right now
    if showImages:
        cv2.imshow('Cleaned Image', cleaned)
        cv2.waitKey(0)
    return cleaned