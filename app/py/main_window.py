###############################################################################
# Imports
###############################################################################

from PySide2.QtWidgets import QApplication, QMainWindow

from importer_widget import ImporterWidget
from hin_lib import HinLib

###############################################################################
# Definitions
###############################################################################

STARTUP_FILE = "C:/HIN/interview/resources/rectangles_2d.png"

###############################################################################
# App
###############################################################################


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setWindowTitle("HIN Interview")
        self.resize(800, 800)

        self.lib = HinLib()

        self.widget = ImporterWidget(self.lib)
        self.setCentralWidget(self.widget)

        self.widget.openFile(STARTUP_FILE)


###############################################################################
# Main
###############################################################################


if __name__ == "__main__":
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
