###############################################################################
# Imports
###############################################################################

import pyqtgraph as pg
from PySide2 import QtCore
from PySide2.QtWidgets import (
    QFileDialog,
    QGridLayout,
    QLabel,
    QLineEdit,
    QPushButton,
    QSpinBox,
    QWidget,
)

###############################################################################
# Definitions
###############################################################################

START_DIR = "C:/HIN/interview/resources"
FILTERS = "Image Files (*.png *.jpg *.bmp *.tif)"

DEFAULT_SPLIT_DIM_X = 100
DEFAULT_SPLIT_DIM_Y = 100

###############################################################################
# Widgets
###############################################################################


class ImporterWidget(QWidget):
    def __init__(self, lib):
        super(ImporterWidget, self).__init__()
        self.lib = lib
        self.createControls()
        self.createLayout()

    ###############################################################################

    def createControls(self):
        plot = pg.PlotItem()
        plot.setLabel(axis="left", text="Y (pixels)")
        plot.setLabel(axis="bottom", text="X (pixels)")

        self.graphWidget = pg.ImageView(view=plot)
        self.graphWidget.ui.roiBtn.hide()
        self.graphWidget.ui.menuBtn.hide()
        self.graphWidget.ui.histogram.hide()

        self.viewBox = self.graphWidget.getView()
        self.viewBox.setAspectLocked()
        self.viewBox.invertY(True)

        self.openFileButton = QPushButton("Open File")
        self.openFileButton.clicked.connect(self.selectFile)

        self.filePathLineEdit = QLineEdit()
        self.filePathLineEdit.setReadOnly(True)

        self.splitDimXBox = QSpinBox()
        self.splitDimXBox.setMaximum(10000)
        self.splitDimXBox.setValue(DEFAULT_SPLIT_DIM_X)
        self.splitDimXBox.setMaximumWidth(50)
        self.splitDimXBox.valueChanged.connect(self.clearCells)
        self.splitDimYBox = QSpinBox()
        self.splitDimYBox.setMaximum(10000)
        self.splitDimYBox.setValue(DEFAULT_SPLIT_DIM_Y)
        self.splitDimYBox.setMaximumWidth(50)
        self.splitDimYBox.valueChanged.connect(self.clearCells)

        self.gridSplitButton = QPushButton("Grid Split")
        self.gridSplitButton.setMaximumWidth(80)
        self.gridSplitButton.clicked.connect(self.gridSplit)

        self.smartSplitButton = QPushButton("Smart Split")
        self.smartSplitButton.setMaximumWidth(80)
        self.smartSplitButton.clicked.connect(self.smartSplit)

        self.imageBorderPen = pg.mkPen({"color": "#FF0", "width": 5})
        self.cellBorderPen = pg.mkPen({"color": "#F0F", "width": 2})
        self.cellOrderPen = pg.mkPen(
            {"color": "#0FF", "width": 1, "style": QtCore.Qt.DashLine}
        )
        self.cellPointsBrush = pg.mkBrush(color="#0FF")

        self.cellItems = []
        self.imageBorder = None

    ###############################################################################

    def createLayout(self):
        layout = QGridLayout()
        layout.addWidget(self.openFileButton, 1, 0)
        layout.addWidget(self.filePathLineEdit, 1, 1)
        layout.addWidget(QLabel("Max Split Dim X"), 2, 0)
        layout.addWidget(self.splitDimXBox, 2, 1)
        layout.addWidget(QLabel("Max Split Dim Y"), 3, 0)
        layout.addWidget(self.splitDimYBox, 3, 1)
        layout.addWidget(self.gridSplitButton, 4, 0)
        layout.addWidget(self.smartSplitButton, 4, 1)
        layout.addWidget(self.graphWidget, 5, 0, 5, 2)

        self.setLayout(layout)

    ###############################################################################

    def clearGraph(self):
        self.graphWidget.clear()
        self.clearCells()
        if self.imageBorder:
            self.viewBox.removeItem(self.imageBorder)

    ###############################################################################

    def clearCells(self):
        for item in self.cellItems:
            self.viewBox.removeItem(item)

    ###############################################################################

    def selectFile(self):
        filepath = QFileDialog.getOpenFileName(
            self, "Select Image", START_DIR, FILTERS
        )[0]
        
        self.openFile(filepath)

    ###############################################################################

    def openFile(self, filepath):
        self.clearGraph()

        self.filePathLineEdit.setText(filepath)

        image = self.lib.openImage(filepath)
        self.plotImage(image)
        self.plotImageBorder(image)

    ###############################################################################

    def gridSplit(self):
        self.clearCells()
    
        cells = self.lib.gridSplit(
            self.splitDimXBox.value(), self.splitDimYBox.value())

        self.plotCells(cells)
        self.plotOrder(cells)

    ###############################################################################

    def smartSplit(self):
    

        self.clearCells()


        cells = self.lib.smartSplit(
            self.splitDimXBox.value(), self.splitDimYBox.value()
        )

        self.plotCells(cells)
        self.plotOrder(cells)

    ###############################################################################

    def plotImage(self, image):
        self.graphWidget.setImage(image.T)

    ###############################################################################

    def plotImageBorder(self, image):
        dimY, dimX = image.shape

        border = pg.PlotCurveItem(
            x=[0, dimX, dimX, 0, 0], y=[0, 0, dimY, dimY, 0], pen=self.imageBorderPen
        )
        self.imageBorder = border
        self.viewBox.addItem(border)

    ###############################################################################

    def plotCells(self, cells):
        for r in cells:
            border = pg.PlotCurveItem(
                x=[
                    r.topLeft.x,
                    r.bottomRight.x,
                    r.bottomRight.x,
                    r.topLeft.x,
                    r.topLeft.x,
                ],
                y=[
                    r.topLeft.y,
                    r.topLeft.y,
                    r.bottomRight.y,
                    r.bottomRight.y,
                    r.topLeft.y,
                ],
                pen=self.cellBorderPen,
            )
            self.cellItems.append(border)
            self.viewBox.addItem(border)

    ###############################################################################

    def plotOrder(self, cells):
        midPointsX, midPointsY = [], []
        for r in cells:
            midPointsX.append((r.topLeft.x + r.bottomRight.x) / 2)
            midPointsY.append((r.topLeft.y + r.bottomRight.y) / 2)

        order = pg.PlotCurveItem(
            x=midPointsX, y=midPointsY, pen=self.cellOrderPen)

        points = pg.ScatterPlotItem(
            x=midPointsX, y=midPointsY, pen=self.cellOrderPen, brush=self.cellPointsBrush)

        self.cellItems.append(order)
        self.cellItems.append(points)

        self.viewBox.addItem(order)
        self.viewBox.addItem(points)
