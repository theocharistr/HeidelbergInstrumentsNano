###############################################################################
# Imports
###############################################################################

import numpy as np
import pyHIN

###############################################################################
# App
###############################################################################


class HinLib(object):
    def __init__(self):
        status = pyHIN.init()
        if status != pyHIN.Status_Success:
            raise RuntimeError("init failed")

        self.importer = pyHIN.getImporter()

    ###############################################################################

    def __del__(self):
        status = pyHIN.quit()
        if status != pyHIN.Status_Success:
            raise RuntimeError("quit failed")

    ###############################################################################

    def openImage(self, filePath):
        status = self.importer.openFile(filePath)
        if status != pyHIN.Status_Success:
            raise RuntimeError("openFile failed")

        dimX = self.importer.getDimX()
        dimY = self.importer.getDimY()

        result = pyHIN.UCharVector(dimX * dimY)
        self.importer.getImage(result)

        image = np.array(result).reshape(dimY, dimX)

        return image

    ###############################################################################

    def gridSplit(self, splitDimXMax, splitDimYMax):
        status = self.importer.gridSplit(splitDimXMax, splitDimYMax)
        if status != pyHIN.Status_Success:
            raise RuntimeError("gridSplit failed")

        numCells = self.importer.getNumCells()

        result = pyHIN.RectVector(numCells)
        self.importer.getCells(result)

        return result

    ###############################################################################

    def smartSplit(self, splitDimXMax, splitDimYMax):
        status = self.importer.smartSplit(splitDimXMax, splitDimYMax)
        if status != pyHIN.Status_Success:
            raise RuntimeError("smartSplit failed")

        numCells = self.importer.getNumCells()

        result = pyHIN.RectVector(numCells)
        self.importer.getCells(result)

        return result
