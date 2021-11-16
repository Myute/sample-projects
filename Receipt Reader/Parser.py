#############################################
# CS478: Assignment 6 - Group portion       #
#                                           #
# Desc: This program takes an image of      #
# receipt and translates it an organized    #
# text format using openCV and Tesseract    #
#                                           #
# File: Parser.py - Parses lists for match  #
#       items and prices                    #
#                                           #
# Authors: Kevyn Higbee, Preston Roberts    #
# Date: May 12, 2021                        #
#############################################
import re

def findItems(words,prices):
    results = []
    for p in prices:
        r = findFromList(p,words)
        if r == '':
            pass
        else:
            results.append([r,p[11]])
    return results

def findKeyWords(words,prices):
    # regex patterns
    patterns = [r'(?i)balance', r'(?i)subtotal', r'(?i)tax', r'(?i)total',r'(?i)change', r'(?i)tax total']
    keyname = ['BALANCE','SUBTOTAL','TAX','TOTAL','CHANGE','TAX TOTAL']
    results = []
    patternnum=-1
    for p in patterns:
        index = 0
        patternnum +=1
        for x in words:
            price = ''
            if re.findall(p,x[11]):
                key = words[index]
                price = findFromList(key, prices)
                words.pop(index)
                if price != '':
                    results.append([keyname[patternnum],price])
                break
            else:
                index += 1

    return results

def findFromList(key,list):
    x,y = int(key[6]), int(key[7])
    if len(list) > 0:
        index = 0
        closeindex = -1        # initialize at invalid index
        dist = 999999999999999 # start with number that will always be larger than furthest distance
        for e in list:
            if ydistance(int(e[7]),y) < dist:
                closeindex = index
                dist = ydistance(int(e[7]),y)
            index += 1
        closest = list[closeindex]
        list.pop(closeindex)
        return closest[11]
    return ''


def ydistance(y2,y1):
    return abs(y2-y1)