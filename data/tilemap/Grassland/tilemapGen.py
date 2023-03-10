import cv2 as cv
import json
import sys
import numpy as np
import os

#ssInfo = sys.argv[0]
#filename = sys.argv[0];

class Line:
    def __init__(self, x1, y1, x2, y2, slope):
        self.x1 = x1
        self.x2 = x2
        self.y1 = y1
        self.y2 = y2
        self.slope = slope
        
    def __str__ (self):
        return("Line going from (" + str(x1) + "," + str(y1) +") to (" + str(x2) + "," + str(y2) + " )\n" + "With dx,dy of (" + str(self.slope[0]) + "," + str(self.slope[1]) + ")")

if len(sys.argv) <= 1:
    print("Missing tilemap sprite sheet description JSON path!")
    quit()

infoFile = open(sys.argv[1], "r")
ssInfo = json.loads(infoFile.read())
img = cv.imread(ssInfo["imageForGeneration"], cv.IMREAD_UNCHANGED)
margin = (int)(ssInfo["margin"])
spacing = (int)(ssInfo["spacing"])
tileHeight = (int)(ssInfo["tileheight"])
tileWidth = (int)(ssInfo["tilewidth"])
orient = ssInfo["direction"]
cols = (int)(ssInfo["columns"])
tileCount = (int)(ssInfo["tilecount"])
outFileName = str(ssInfo["generationDescription"])
infoFile.close()

jsonList = []


for i in range(1, tileCount + 1):
    jsonObj =  {
                "hillOrientation" : None,
                "possibleConnects": [],
                "layers" : [],
                "tileIndex" : i-1
                }

    layers = []
    xStart = (int)(tileWidth * (i % cols) + spacing * (i % cols) + margin)
    yStart = (int)(tileHeight * int(i / cols) + spacing * int(i / cols) + margin)
    
    
    ################################
    #1.----------------------------#
    ################################
    #Scan the top-left -> top-right#
    ################################
    x1 = xStart
    x2 = xStart + tileWidth - 1
    y1 = 0
    y2 = 0
    slope = np.array([0,0])

    for y in range(yStart, yStart + tileHeight):
        if img[y, x1][3] != 0:
            y1 = y
            for endY in range(yStart, yStart + tileHeight ):
                if img[endY,x2][3] != 0:
                    y2 = endY
                    break
            break
            
    
    slope[0] = x2 - x1
    slope[1] = abs(y2 - y1)
    dx = slope[0]
    dy = slope[1]
    err = dx / 2
    yStep = (y1 < y2) if 1 else -1
    steep = (abs(y2 - y1) > abs(x2 - x1))
    nSlope = np.linalg.norm(slope)
    validCoord1 = False
    validCoord2 = False
    
    topSlope = Line(x1, y1, x2, y2, slope)
    
            
    ####################################
    #2.--------------------------------#
    ####################################
    #Scan the top-right -> bottom-right#
    ####################################
    x1 = xStart + tileWidth - 1
    x2 = xStart + tileWidth - 1 
    y1 = yStart
    y2 = yStart + tileHeight - 1
    slope = np.array([0,0])
    
    for x in range(xStart + tileWidth, xStart, -1):
        if img[y1, x][3] != 0:
            x1 = x
            validCoord1 = True
            for endX in range(xStart + tileWidth - 1, xStart, -1):
                if img[y2, endX][3] != 0:
                    x2 = endX
                    break
            break
       
    
    slope[0] = x2 - x1
    slope[1] = abs(y2 - y1)
    dx = slope[0]
    dy = slope[1]
    err = dx / 2
    yStep = (y1 < y2) if 1 else -1
    steep = (abs(y2 - y1) > abs(x2 - x1))
    nSlope = np.linalg.norm(slope)
    validCoord1 = False
    validCoord2 = False
    
    rightSlope = Line(x1, y1, x2, y2, slope)


            
    ######################################
    #3.----------------------------------#
    ######################################
    #Scan the bottom-left -> bottom-right#
    ######################################
    x1 = xStart
    x2 = xStart + tileWidth - 1 
    y1 = 0
    y2 = 0
    slope = np.array([0,0])

    for y in range(yStart + tileHeight - 1, yStart, -1):
        if img[y, x1][3] != 0:
            y1 = y
            for endY in range(yStart + tileHeight - 1, yStart, -1):
                if img[endY, x2][3] != 0:
                    y2 = endY
                    break
            break
        
    slope[0] = x2 - x1
    slope[1] = abs(y2 - y1)
    dx = slope[0]
    dy = slope[1]
    err = dx / 2
    yStep = (y1 < y2) if 1 else -1
    steep = (abs(y2 - y1) > abs(x2 - x1))
    nSlope = np.linalg.norm(slope)
    validCoord1 = False
    validCoord2 = False
    
    bottomSlope = Line(x1, y1, x2, y2, slope)


    ##################################
    #4.------------------------------#
    ##################################
    #Scan the top-left -> bottom-left#
    ##################################
    x1 = xStart
    x2 = xStart
    y1 = yStart
    y2 = yStart + tileHeight - 1
    slope = np.array([0,0])
    
    for x in range(xStart,  xStart + tileWidth - 1):
        if img[y1, x][3] != 0:
            x1 = x
            for endX in range(xStart,  xStart + tileWidth - 1):
                if img[y2, endX][3] != 0:
                    x2 = endX
                    break
            break
        
    slope[0] = x2 - x1
    slope[1] = abs(y2 - y1)
    dx = slope[0]
    dy = slope[1]
    err = dx / 2
    yStep = (y1 < y2) if 1 else -1
    steep = (abs(y2 - y1) > abs(x2 - x1))
    nSlope = np.linalg.norm(slope)

    leftSlope = Line(x1, y1, x2, y2, slope)


    #########################
    #########################
    ##DETERMINE CONNECTIONS##
    #########################
    #########################

    
#########################
#Check NW, N, NE  #######
#########################

        #   *
        #   _
        #  | |
        #   -
        #
    if topSlope.slope[1] == 0:
        layers.append("ground")
        layers.append("wall")
        #
        # * _
        #  | |
        #   -
        #
        if leftSlope.slope[0] == 0:
            jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "hill", "ground", "wall"
                    ],
                "x" : -1,
                "y" : -1
            })
        else:
            layers.append("platform")
            #
            # * _
            #  \ |
            #   -
            #
            if leftSlope.slope[0] < 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : -1,
                    "y" : -1
                })
                
            #
            # * _
            #  / |
            #   -
            #
            elif leftSlope.slope[0] > 0:
                layers.append("hill")
                if "wall" in layers:
                    layers.remove("wall")
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground", "wall"
                        ],
                    "x" : -1,
                    "y" : -1
                })

            
        #
        #   _ *
        #  | |
        #   -
        #
        if rightSlope.slope[0] == 0:
            jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "hill", "ground", "wall"
                    ],
                "x" : 1,
                "y" : -1
            })
        else:
            #
            #   _ *
            #  | /
            #   -
            #
            if rightSlope.slope[0] < 0 :
                layers.append("platform")
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground", "wall"
                        ],
                    "x" : 1,
                    "y" : -1
                })

            #
            #   _ *
            #  | \
            #   -
            #
            elif rightSlope.slope[0] > 0:
                layers.append("platform")
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : 1,
                    "y" : -1
                })
        #   *
        #   _ 
        #  | |
        #   -
        #
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "hill", "ground", "wall"
                    ],
                "x" : 0,
                "y" : -1
            })
    else:
        #   *
        #   / 
        #  | |
        #   -
        #
        if topSlope.y2 - topSlope.y1 < 0:
            layers.append("hill")
            if "wall" in layers:
                layers.remove("wall")
            jsonObj["hillOrientation"] = "Northeast"
            #
            # * /
            #  \ |
            #   -
            #
            if leftSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground", "wall"
                        ],
                    "x" : -1,
                    "y" : -1
                })
            #
            # * /
            #  | |
            #   -
            #
            elif leftSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground", "wall"
                        ],
                    "x" : -1,
                    "y" : -1
                })

            #
            #   / *
            #  | /
            #   -
            #
            if rightSlope.slope[0] < 0 :
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "wall"
                        ],
                    "x" : 1,
                    "y" : -1
                })

            #
            #   / *
            #  | \
            #   -
            #
            elif rightSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : 1,
                    "y" : -1
                })

            #
            #   / *
            #  | |
            #   -
            #    
            elif rightSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground", "wall"
                        ],
                    "x" : 1,
                    "y" : -1
                })
                
            
        #   *
        #   \ 
        #  | |
        #   -
        #
        elif topSlope.y2 - topSlope.y1 > 0:
            layers.append("hill")
            if "wall" in layers:
                layers.remove("wall")
            jsonObj["hillOrientation"] = "Southeast"
            
            #
            # * \
            #  \ |
            #   -
            #
            if leftSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill"
                        ],
                    "x" : -1,
                    "y" : -1
                })
            #
            # * \
            #  | |
            #   -
            #
            elif leftSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground", "wall"
                        ],
                    "x" : -1,
                    "y" : -1
                })

            #
            #   \ *
            #  | /
            #   -
            #
            if rightSlope.slope[0] < 0 :
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill",
                        ],
                    "x" : 1,
                    "y" : -1
                })

            #
            #   \ *
            #  | \
            #   -
            #
            elif rightSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : 1,
                    "y" : -1
                })

            #
            #   \ *
            #  | |
            #   -
            #    
            elif rightSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "wall"
                        ],
                    "x" : 1,
                    "y" : -1
                })








#########################
#Check SW, S, SE  #######
#########################

    #   
    #   _
    #  | |
    #   -
    #   *
    if bottomSlope.slope[1] == 0:
        #
        #   _
        #  | |
        # * -
        #
        if leftSlope.slope[0] == 0:
            jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "hill", "ground", "wall"
                    ],
                "x" : -1,
                "y" : 1
            })
        else:
            #
            #   _
            #  \ |
            # * -
            #
            if leftSlope.slope[0] < 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground"
                        ],
                    "x" : -1,
                    "y" : 1
                })
                
            #
            #   _
            #  / |
            # * -
            #
            elif leftSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground"
                        ],
                    "x" : -1,
                    "y" : 1
                })

            
        #
        #   _ 
        #  | |
        #   - *
        #
        if rightSlope.slope[0] == 0:
            jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "hill", "ground"
                    ],
                "x" : 1,
                "y" : 1
            })
        else:
            #
            #   _ 
            #  | /
            #   - *
            #
            if rightSlope.slope[0] < 0 :
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground"
                        ],
                    "x" : 1,
                    "y" : 1
                })

            #
            #   _ 
            #  | \
            #   - *
            #
            elif rightSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill"
                        ],
                    "x" : 1,
                    "y" : 1
                })
        #   
        #   _ 
        #  | |
        #   -
        #   *
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground"
                    ],
                "x" : 0,
                "y" : 1
            })
    else:
        #   
        #   - 
        #  | |
        #   /
        #   *
        if bottomSlope.y2 - topSlope.y1 < 0:
            #
            #   -
            #  \ |
            # * /
            #
            if leftSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : -1,
                    "y" : 1
                })
            #
            #   -
            #  | |
            # * /
            #
            elif leftSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground"
                        ],
                    "x" : -1,
                    "y" : 1
                })

            #
            #   -
            #  | /
            #   / *
            #
            if rightSlope.slope[0] < 0 :
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground"
                        ],
                    "x" : 1,
                    "y" : 1
                })

            #
            #   -
            #  | \
            #   / *
            #
            elif rightSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill", "ground"
                        ],
                    "x" : 1,
                    "y" : 1
                })

            #
            #   -
            #  | /
            #   / *
            #    
            elif rightSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground"
                        ],
                    "x" : 1,
                    "y" : 1
                })
                
            
        #   
        #   - 
        #  | |
        #   \
        #   *
        elif bottomSlope.y2 - topSlope.y1 > 0:
            #
            #   -
            #  \ |
            # * \
            #
            if leftSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground"
                        ],
                    "x" : -1,
                    "y" : 1
                })
            #  
            #   -
            #  | |
            # * \
            #
            elif leftSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground", "wall"
                        ],
                    "x" : -1,
                    "y" : 1
                })

            #
            #   -
            #  | /
            #   \ *
            #
            if rightSlope.slope[0] < 0 :
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        ],
                    "x" : 1,
                    "y" : 1
                })

            #
            #   -
            #  | \
            #   \ *
            #
            elif rightSlope.slope[0] > 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "hill"
                        ],
                    "x" : 1,
                    "y" : 1
                })

            #
            #   -
            #  | |
            #   \ *
            #    
            elif rightSlope.slope[0] == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "ground", "hill"
                        ],
                    "x" : 1,
                    "y" : 1
                })

            
#########################
#Check W, E  #######
#########################

## WEST
    #   
    #   _
    # *| |
    #   -
    #   
    if leftSlope.slope[0] == 0:
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground", "hill", "wall"
                    ],
                "x" : -1,
                "y" : 0
            })
    #   
    #   _
    # */ |
    #   -
    #   
    elif leftSlope.x2 - leftSlope.x1 > 0:
        jsonObj["hillOrientation"] = "Northeast"
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    ],
                "x" : -1,
                "y" : 0
            })

    #   
    #   _
    # *\ |
    #   -
    #               
    elif leftSlope.x2 - leftSlope.x1 < 0:
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground", "hill"
                    ],
                "x" : -1,
                "y" : 0
            })

## EAST
    #   
    #   _
    #  | |*
    #   -
    #   
    if leftSlope.slope[0] == 0:
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground", "hill", "wall"
                    ],
                "x" : 1,
                "y" : 0
            })
    #   
    #   _
    #  | \*
    #   -
    #   
    elif leftSlope.x2 - leftSlope.x1 > 0:
        jsonObj["hillOrientation"] = "Southeast"
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    ],
                "x" : 1,
                "y" : 0
            })

    #   
    #   _
    #  | /*
    #   -
    #               
    elif leftSlope.x2 - leftSlope.x1 < 0:
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground", "hill"
                    ],
                "x" : 1,
                "y" : 0
            })

    jsonObj["layers"] = layers
            
    jsonList.append(jsonObj)



with open(outFileName, 'w', encoding='utf-8') as outfile:
    json.dump(jsonList, outfile, ensure_ascii=False, indent=4, sort_keys=True)

print("Done")
