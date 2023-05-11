import cv2 as cv
import json
import sys
import numpy as np
import os

ssInfo = sys.argv[0]
filename = sys.argv[0];

if len(sys.argv) <= 1:
    print("Missing tilemap sprite sheet description JSON path!")
    quit()



infoFile = open(sys.argv[1], "r")
ssInfo = json.loads(infoFile.read())
img = cv.imread(ssInfo["imageForDescription"], cv.IMREAD_UNCHANGED)
margin = (int)(ssInfo["margin"])
spacing = (int)(ssInfo["spacing"])
tileHeight = (int)(ssInfo["tileheight"])
tileWidth = (int)(ssInfo["tilewidth"])
imageHeight = (int)(ssInfo["imageheight"])
imageWidth = (int)(ssInfo["imagewidth"])
orient = ssInfo["direction"]
cols = (int)(ssInfo["columns"])
tileCount = (int)(ssInfo["tilecount"])
outFileName = str(ssInfo["generationDescriptionOut"])
name = str(ssInfo["name"])
infoFile.close()

SAMPLES = 16

jsonList = []

tileIndex = 1

print("program start")

while tileIndex in range(1, tileCount+1):
    jsonObj =  {
                "hillOrientation" : "None",
                "possibleConnects": [],
                "layers" : [],
                "tileIndex" : tileIndex,
                "xLocation": int(tileWidth * int(tileIndex % cols) + spacing * int(tileIndex % cols) + margin),                        
                "yLocation": int(tileHeight * int(tileIndex / cols) + spacing * int(tileIndex / cols) + margin)                     
                }

    slopes = {
        "dx": 0,
        "dy": 0,
        "slope": 0
        }

    layers = []
    xStart = jsonObj["xLocation"]
    yStart = jsonObj["yLocation"]

    
    #################################
    ##1.----------------------------#
    #################################
    ##Scan the top-left -> top-right#
    #################################
    xyDictionary = {
        "x": np.array([]),
        "y" : np.array([])
        }
    x = xStart
    y = yStart
    
    slopes = np.array([])


    while x in range(xStart, xStart+tileWidth) and y in range(yStart, yStart+tileHeight):
        if img[y,x][3] > 0:
            xyDictionary["y"] = np.append(xyDictionary["y"], img.shape[0] - y - 1)
            x += tileWidth // SAMPLES
            y = yStart
        else:
            y += 1
        if y >= yStart+tileHeight-1:
            x += tileWidth // SAMPLES
            y = yStart

    i = 0
    while i in range(len(xyDictionary["y"])-1):
        slopes = np.append(slopes, xyDictionary["y"][i+1] - xyDictionary["y"][i])
        i += 1
        
    topAvgSlope = np.average(slopes)
    
    ####################################
    #2.--------------------------------#
    ####################################
    #Scan the top-right -> bottom-right#
    ####################################

    xyDictionary = {
        "x": np.array([]),
        "y" : np.array([])
    }
    x = xStart + tileWidth - 1
    y = yStart
  

    slopes = np.array([])

    while y in range(yStart, yStart+tileHeight) and x in range(xStart, xStart+tileWidth):
        if img[y,x][3] > 0:
            xyDictionary["x"] = np.append(xyDictionary["x"], x)
            y += tileHeight // SAMPLES
            x = xStart + tileWidth - 1
        elif x <= xStart:
            y += tileHeight // SAMPLES
            x = xStart + tileWidth - 1
        else:
            x -= 1
    
    i = 0
    while i in range(len(xyDictionary["x"])-1):
        slopes = np.append(slopes, xyDictionary["x"][i+1] - xyDictionary["x"][i])
        i += 1
        
    rightAvgSlope = np.average(slopes)
            
    ######################################
    #3.----------------------------------#
    ######################################
    #Scan the bottom-left -> bottom-right#
    ######################################

    xyDictionary = {
        "x": np.array([]),
        "y" : np.array([])
    }
    x = xStart
    y = yStart + tileHeight - 1
    
    slopes = np.array([])

    while x in range(xStart, xStart+tileWidth) and y in range(yStart, yStart+tileHeight):
        if img[y,x][3] > 0:
            xyDictionary["y"] = np.append(xyDictionary["y"], img.shape[0] - y - 1)
            x += tileWidth // SAMPLES
            y = yStart + tileHeight - 1
        else:
            y -= 1
        if y <= yStart:
            x += tileWidth // SAMPLES
            y = yStart + tileHeight - 1

    i = 0
    while i in range(len(xyDictionary["y"])-1):
        slopes = np.append(slopes, xyDictionary["y"][i+1] - xyDictionary["y"][i])
        i += 1
        
    bottomAvgSlope = np.average(slopes)

    ##################################
    #4.------------------------------#
    ##################################
    #Scan the top-left -> bottom-left#
    ##################################

    xyDictionary = {
        "x": np.array([]),
        "y" : np.array([])
    }
    x = xStart
    y = yStart
    
    slopes = np.array([])

    while y in range(yStart, yStart+tileHeight) and x in range(xStart, xStart+tileWidth):
        if img[y,x][3] > 0:
            xyDictionary["x"] = np.append(xyDictionary["x"], x)
            y += tileHeight // SAMPLES
            x = xStart
        elif x >= xStart+tileWidth-1:
            y += tileHeight // SAMPLES
            x = xStart
        else:
            x += 1

    i = 0
    while i in range(len(xyDictionary["x"])-1):
        slopes = np.append(slopes, xyDictionary["x"][i+1] - xyDictionary["x"][i])
        i += 1

    leftAvgSlope = np.average(slopes)
    
    #########################
    #########################
    ##DETERMINE CONNECTIONS##
    #########################
    #########################

   #print(f"Top {topAvgSlope} Bottom {bottomAvgSlope} Left {leftAvgSlope} Right {rightAvgSlope}")

    
#########################
#Check NW, N, NE  #######
#########################

    #   *
    #   _
    #  | |
    #   -
    #
    if topAvgSlope == 0:
        layers.append("ground")
        layers.append("wall")
        #
        # * _
        #  | |
        #   -
        #
        if leftAvgSlope == 0:
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
            if leftAvgSlope < 0:
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
            elif leftAvgSlope > 0:
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
        if rightAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
        if topAvgSlope < 0:
            layers = ["hill"]
            jsonObj["hillOrientation"] = "Northeast"
            #
            # * /
            #  \ |
            #   -
            #
            if leftAvgSlope > 0:
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
            elif leftAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
            elif rightAvgSlope == 0:
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
        elif topAvgSlope > 0:
            layers = ["hill"]
            jsonObj["hillOrientation"] = "Southeast"
            
            #
            # * \
            #  \ |
            #   -
            #
            if leftAvgSlope > 0:
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
            elif leftAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
            elif rightAvgSlope == 0:
                jsonObj["possibleConnects"].append({
                    "allowedLayers" : [
                        "wall"
                        ],
                    "x" : 1,
                    "y" : -1
                })



    ##print(topAvgSlope, bottomAvgSlope, leftAvgSlope, rightAvgSlope, '\t', tileIndex)




#########################
#Check SW, S, SE  #######
#########################

    #   
    #   _
    #  | |
    #   -
    #   *
    if bottomAvgSlope == 0:
        #
        #   _
        #  | |
        # * -
        #
        if leftAvgSlope == 0:
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
            if leftAvgSlope < 0:
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
            elif leftAvgSlope > 0:
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
        if rightAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
        if bottomAvgSlope < 0:
            #
            #   -
            #  \ |
            # * /
            #
            if leftAvgSlope > 0:
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
            elif leftAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
            elif rightAvgSlope == 0:
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
        elif bottomAvgSlope > 0:
            #
            #   -
            #  \ |
            # * \
            #
            if leftAvgSlope > 0:
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
            elif leftAvgSlope == 0:
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
            if rightAvgSlope < 0 :
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
            elif rightAvgSlope > 0:
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
            elif rightAvgSlope == 0:
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
    if leftAvgSlope == 0:
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
    elif leftAvgSlope > 0:
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
    elif leftAvgSlope < 0:
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
    if rightAvgSlope == 0:
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
    elif rightAvgSlope > 0:
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
    elif rightAvgSlope < 0:
        jsonObj["possibleConnects"].append({
                "allowedLayers" : [
                    "ground", "hill"
                    ],
                "x" : 1,
                "y" : 0
            })

    jsonObj["layers"] = layers
            
    jsonList.append(jsonObj)

    tileIndex += 1


with open(outFileName, 'w', encoding='utf-8') as outfile:
    json.dump({name: jsonList}, outfile, ensure_ascii=False, indent=4, sort_keys=True)

print("Done")
